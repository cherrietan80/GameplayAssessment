// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAssessmentCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameplayAssessment.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "AttributeSets/BasicAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Weapons/BaseWeapon_Ranged.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Weapons/WeaponManagerComponent.h"


void AGameplayAssessmentCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(
				UBasicAttributeSet::GetStaminaAttribute())
			.AddUObject(this, &AGameplayAssessmentCharacter::OnStaminaChanged);
	}
	TargetLength = MaxLength;
	TargetSocketOffset = FVector::ZeroVector;
}

AGameplayAssessmentCharacter::AGameplayAssessmentCharacter() //constructor
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = NormalMaxSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = MaxLength;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

}

void AGameplayAssessmentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyEqual(CameraBoom->TargetArmLength, TargetLength, 0.1f))
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(
			CameraBoom->TargetArmLength,
			TargetLength,
			DeltaTime,
			8.f);
	}

	if (!CameraBoom->SocketOffset.Equals(TargetSocketOffset, 0.1f))
	{
		CameraBoom->SocketOffset = FMath::VInterpTo(
			CameraBoom->SocketOffset,
			TargetSocketOffset,
			DeltaTime,
			8.f
		);
	}
}

void AGameplayAssessmentCharacter::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	const float OldStamina = Data.OldValue;
	const float NewStamina = Data.NewValue;
	const float MaxStamina = BasicAttributeSet->GetMaxStamina();

	if(NewStamina < OldStamina)
	{
		FGameplayTagContainer Container;
		Container.AddTag(TAG_GameplayEffect_StaminaRegen);

		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(Container);

		GetWorld()->GetTimerManager().SetTimer(
			StaminaRegenDelayTimer,
			this,
			&AGameplayAssessmentCharacter::StartStaminaRegen,
			1.0f,
			false
		);
		
	}
	else if(NewStamina >= MaxStamina)
	{
		FGameplayTagContainer Container;
		Container.AddTag(TAG_GameplayEffect_StaminaRegen);

		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(Container);
	}
	
}

void AGameplayAssessmentCharacter::StartStaminaRegen()
{
	AbilitySystemComponent->ApplyGameplayEffectToSelf(StaminaRegenEffect.GetDefaultObject(), 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void AGameplayAssessmentCharacter::EnterAimMode()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = AimedMaxSpeed;

	TargetLength = MinLength;
	TargetSocketOffset = AimedSocketOffset;

	ABaseWeapon_Ranged* RangedWeapon = Cast<ABaseWeapon_Ranged>(WeaponManagerComponent->GetEquippedWeapon());
	if (!RangedWeapon) return;
	bIsAiming = true;
	
}

void AGameplayAssessmentCharacter::ExitAimMode()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = NormalMaxSpeed;

	TargetLength = MaxLength;
	TargetSocketOffset = FVector::ZeroVector;

	ABaseWeapon_Ranged* RangedWeapon = Cast<ABaseWeapon_Ranged>(WeaponManagerComponent->GetEquippedWeapon());
	if (!RangedWeapon) return;
	bIsAiming = false;
}

void AGameplayAssessmentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameplayAssessmentCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGameplayAssessmentCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameplayAssessmentCharacter::Look);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::DoDash);

		//Melee
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::ToggleEquipMelee);
		EnhancedInputComponent->BindAction(MeleeSwingAttackAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::DoMeleeAttackSwing);
		EnhancedInputComponent->BindAction(MeleeComboAttackAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::DoMeleeAttackCombo);

		//Ranged
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::ToggleEquipRanged);
		EnhancedInputComponent->BindAction(RangedShootAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::DoRangedShooting);
		EnhancedInputComponent->BindAction(RangedAimAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::DoRangedAim);
		EnhancedInputComponent->BindAction(RangedAimAction, ETriggerEvent::Completed, this, &AGameplayAssessmentCharacter::CancelRangedAim);
		EnhancedInputComponent->BindAction(RangedReloadAction, ETriggerEvent::Started, this, &AGameplayAssessmentCharacter::DoRangedReload);
	}
	else
	{
		UE_LOG(LogGameplayAssessment, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGameplayAssessmentCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AGameplayAssessmentCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGameplayAssessmentCharacter::DoDash()
{
	ActivateAbilityByTag(TAG_Ability_Dash);
}

void AGameplayAssessmentCharacter::DoMeleeAttackSwing()
{
	ActivateAbilityByTag(TAG_Ability_MeleeAttack_Swing);
}

void AGameplayAssessmentCharacter::DoMeleeAttackCombo()
{
	ActivateAbilityByTag(TAG_Ability_MeleeAttack_Combo);

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_ContinueCombo_Input;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Data.EventTag,
		Data
	);
}

void AGameplayAssessmentCharacter::DoRangedShooting()
{
	ActivateAbilityByTag(TAG_Ability_Ranged_Shoot);
}

void AGameplayAssessmentCharacter::DoRangedAim()
{
	ActivateAbilityByTag(TAG_Ability_Ranged_Aim);
}

void AGameplayAssessmentCharacter::CancelRangedAim()
{
	CancelAbilityByTag(TAG_Ability_Ranged_Aim);
	UE_LOG(LogTemp, Warning, TEXT("CancelRangedAim called"));
}

void AGameplayAssessmentCharacter::DoRangedReload()
{
	ActivateAbilityByTag(TAG_Ability_Ranged_Reload);
}

void AGameplayAssessmentCharacter::ToggleEquipMelee()
{
	if (!WeaponManagerComponent)
		return;

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_Weapon_Equip;
	FGameplayEventData Payload;
	Payload.TargetTags.AddTag(TAG_Weapon_Melee);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Data.EventTag,
		Payload
	);
}

void AGameplayAssessmentCharacter::ToggleEquipRanged()
{
	if (!WeaponManagerComponent)
		return;
	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_Weapon_Equip;
	FGameplayEventData Payload;
	Payload.TargetTags.AddTag(TAG_Weapon_Ranged);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Data.EventTag,
		Payload
	);
}



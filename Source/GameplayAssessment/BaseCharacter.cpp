// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "AttributeSets/BasicAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/BaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/WeaponManagerComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Add the ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));

	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_State_Death).
		AddUObject(this, &ABaseCharacter::OnDeadTagChanged);

	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManagerComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveAbilities(StartingAbilities);
	}
}

void ABaseCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ABaseCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ABaseCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ABaseCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

TArray<FGameplayAbilitySpecHandle> ABaseCharacter::GiveAbilities(TArray<TSubclassOf<class UGameplayAbility>> AbilitiesToGranted)
{
	if (!AbilitySystemComponent)
	{
		return TArray<FGameplayAbilitySpecHandle>();
	}
	
	TArray<FGameplayAbilitySpecHandle> GrantedHandles;

	for (TSubclassOf<UGameplayAbility>Ability : AbilitiesToGranted)
	{
		if (Ability)
		{
			FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1));
			GrantedHandles.Add(SpecHandle);
		}
	}

	SendAbilitiesChangedEvent();

	return GrantedHandles;
}

void ABaseCharacter::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for(FGameplayAbilitySpecHandle Handle : AbilitiesToRemove)
	{
		if(Handle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(Handle);

		}
	}

	SendAbilitiesChangedEvent();
}

void ABaseCharacter::ActivateAbilityByTag(FGameplayTag Tag)
{
	if (!AbilitySystemComponent) return;

	FGameplayTagContainer Container;
	Container.AddTag(Tag);

	AbilitySystemComponent->TryActivateAbilitiesByTag(Container);
}

void ABaseCharacter::CancelAbilityByTag(FGameplayTag Tag)
{
	if (!AbilitySystemComponent) return;

	FGameplayTagContainer Container;
	Container.AddTag(Tag);

	AbilitySystemComponent->CancelAbilities(&Container);
}

void ABaseCharacter::SendAbilitiesChangedEvent()
{
	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_AbilityChanged;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		Data.EventTag,
		Data
	);

}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::OnDeadTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		HandleDeath();
	}
}

void ABaseCharacter::HandleDeath()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	FVector Impulse = GetActorForwardVector() * -20000;
	Impulse.Z = 15000;
	GetMesh()->AddImpulseAtLocation(Impulse, GetActorLocation());
}

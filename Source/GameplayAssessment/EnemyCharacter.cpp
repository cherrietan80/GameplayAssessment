// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon.h"

AEnemyCharacter::AEnemyCharacter()
{
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetMesh());
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy Called"));
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AEnemyCharacter::EquipWeapon);

	GetWorldTimerManager().SetTimer(
		AttackTimer,
		this,
		&AEnemyCharacter::Attack,
		5.0f,
		true
	);

	CreateHealthWidget();
}

void AEnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	OnEnemyDeath.Broadcast();

	FTimerHandle DestroyDelay;

	GetWorldTimerManager().SetTimer(
		DestroyDelay,
		this,
		&AEnemyCharacter::DestroyActor,
		1.f,
		false
	);
}

void AEnemyCharacter::EquipWeapon()
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

void AEnemyCharacter::Attack()
{
	if (!AbilitySystemComponent) return;
	ActivateAbilityByTag(TAG_Ability_MeleeAttack_Sweep);
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void AEnemyCharacter::DestroyActor()
{
    if (WeaponManagerComponent)
    {
        ABaseWeapon* EquippedWeapon = WeaponManagerComponent->GetEquippedWeapon();
        if (EquippedWeapon)
        {
            EquippedWeapon->Destroy();
        }
    }
    Destroy();
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Ranged_Reload.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon_Ranged.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"

UGA_Ranged_Reload::UGA_Ranged_Reload()
{
	AbilityTags.AddTag(TAG_Ability_Ranged_Reload);
	bShouldShowInAbilityBar = true;
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Ranged_Shoot);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Base);
}

void UGA_Ranged_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	UWeaponManagerComponent* WeaponManagerComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UWeaponManagerComponent>();
	
	if (!WeaponManagerComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	EquippedWeapon = WeaponManagerComponent->GetEquippedWeapon();

	if (!EquippedWeapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	ABaseWeapon_Ranged* RangedWeapon = Cast<ABaseWeapon_Ranged>(EquippedWeapon);

	if (RangedWeapon->GetAmmoAmount() >= RangedWeapon->GetAmmoPerMagazineAmount())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	RangedWeapon->FullFillAmmo();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

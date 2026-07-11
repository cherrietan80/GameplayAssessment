// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_Ranged_Shoot.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon_Ranged.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"

UGA_Ranged_Shoot::UGA_Ranged_Shoot()
{
	AbilityTags.AddTag(TAG_Ability_Ranged_Shoot);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	bShouldShowInAbilityBar = true;
}

void UGA_Ranged_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	UWeaponManagerComponent* WeaponManagerComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UWeaponManagerComponent>();
	
	if (!WeaponManagerComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	EquippedWeapon = WeaponManagerComponent->GetEquippedWeapon();

	if(!EquippedWeapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	ABaseWeapon_Ranged* RangedWeapon = Cast<ABaseWeapon_Ranged>(EquippedWeapon);

	if (!RangedWeapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if (RangedWeapon->GetAmmoAmount() <= 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectClass,
		1.f,
		Context);

	SpecHandle.Data->SetSetByCallerMagnitude(
		TAG_Data_Damage,
		RangedWeapon->GetDamageAmount()
	);

	RangedWeapon->EffectSpec = *SpecHandle.Data.Get();

	RangedWeapon->Fire();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

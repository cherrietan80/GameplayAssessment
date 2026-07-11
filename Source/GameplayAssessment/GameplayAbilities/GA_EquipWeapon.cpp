// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_EquipWeapon.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "Weapons/WeaponManagerComponent.h"
#include <GameplayAssessmentCharacter.h>

UGA_EquipWeapon::UGA_EquipWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;

	TriggerData.TriggerTag = TAG_GameplayEvent_Weapon_Equip;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UGA_EquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (TriggerEventData)
	{
		FGameplayTag TargetTag;

		// Get first tag
		if (TriggerEventData->TargetTags.Num() > 0)
		{
			TargetTag = TriggerEventData->TargetTags.GetByIndex(0);
		}

		if (TargetTag.IsValid())
		{
			UWeaponManagerComponent* WeaponManager = GetAvatarActorFromActorInfo()->GetComponentByClass<UWeaponManagerComponent>();
			AGameplayAssessmentCharacter* Character = Cast<AGameplayAssessmentCharacter>(GetAvatarActorFromActorInfo());

			if (WeaponManager && Character)
			{
				if (TargetTag == TAG_Weapon_Melee)
				{
					WeaponManager->ToggleEquipWeapon(MeleeWeaponClass);
				}
				else if (TargetTag == TAG_Weapon_Ranged)
				{
					WeaponManager->ToggleEquipWeapon(RangedWeaponClass);
				}
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

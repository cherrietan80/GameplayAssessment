// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Death.h"
#include "GameAbilitiesGameplayTags.h"

UGA_Death::UGA_Death()
{
	AbilityTags.AddTag(TAG_Ability_Death);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Base);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Base);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyGameplayEffectToOwner(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		DeathEffect.GetDefaultObject(),
		1.0f
	);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

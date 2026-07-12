// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_HitReaction.h"
#include "GameAbilitiesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_HitReaction::UGA_HitReaction()
{
	AbilityTags.AddTag(TAG_Ability_HitReaction);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_Base);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGA_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HitMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		HitMontage
	);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_HitReaction::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_HitReaction::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_HitReaction::OnMontageCancelled);

	MontageTask->ReadyForActivation();
}

void UGA_HitReaction::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_HitReaction::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_HitReaction::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

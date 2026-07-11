// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_Ranged_Base.h"
#include "GA_Ranged_Aim.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_Ranged_Aim : public UGA_Ranged_Base
{
	GENERATED_BODY()
	
public:
	UGA_Ranged_Aim();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;
};

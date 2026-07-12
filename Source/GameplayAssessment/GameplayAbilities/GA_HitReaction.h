// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/BaseGameplayAbility.h"
#include "GA_HitReaction.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_HitReaction : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_HitReaction();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	void OnMontageCompleted();
	void OnMontageInterrupted();
	void OnMontageCancelled();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* HitMontage;
};

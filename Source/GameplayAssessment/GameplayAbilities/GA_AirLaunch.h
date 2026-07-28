// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/BaseGameplayAbility.h"
#include "GA_AirLaunch.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_AirLaunch : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_AirLaunch();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;


protected:

	UPROPERTY(EditDefaultsOnly)
	float LaunchDuration = 0.25f;

protected:
	UFUNCTION(BlueprintCallable)
	void FinishLaunch();
};

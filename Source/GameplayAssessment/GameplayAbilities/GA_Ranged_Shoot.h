// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_Ranged_Base.h"
#include "GA_Ranged_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_Ranged_Shoot : public UGA_Ranged_Base
{
	GENERATED_BODY()

public:
	UGA_Ranged_Shoot();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

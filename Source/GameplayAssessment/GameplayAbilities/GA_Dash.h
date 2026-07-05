// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_Dash : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_Dash();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData
	) override;

public:
	UFUNCTION(BlueprintPure, Category = "Dash")
	FVector GetDashDirection() const;

	UFUNCTION(BlueprintPure, Category = "Dash")
	float GetMaxSpeed() const;

private:
	UFUNCTION()
	void OnDashFinished();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float Strength = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float Duration = .3f;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/BaseGameplayAbility.h"
#include "GA_MeleeAttack_Swing.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_MeleeAttack_Swing : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_MeleeAttack_Swing();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* MeleeAttackSwingMontage;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();
	
};

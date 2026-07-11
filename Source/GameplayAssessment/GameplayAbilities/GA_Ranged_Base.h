// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/BaseGameplayAbility.h"
#include "Weapons/BaseWeapon.h"
#include "GA_Ranged_Base.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_Ranged_Base : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Ranged_Base();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	virtual void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* ActionMontage;

	ABaseWeapon* EquippedWeapon;
	
};

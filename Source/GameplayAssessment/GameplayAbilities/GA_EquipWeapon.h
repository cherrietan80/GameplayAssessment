// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilities/BaseGameplayAbility.h"
#include "GA_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_EquipWeapon : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_EquipWeapon();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class ABaseWeapon> MeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class ABaseWeapon> RangedWeaponClass;
};

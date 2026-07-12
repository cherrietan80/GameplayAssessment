// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_MeleeAttack_Base.h"
#include "GA_MeleeAttack_Combo.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_MeleeAttack_Combo : public UGA_MeleeAttack_Base
{
	GENERATED_BODY()
	
public:
	UGA_MeleeAttack_Combo();

protected:
	virtual void MontageStart() override;

	virtual void CalculateDamageAmount() override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

private:
	UPROPERTY()
	bool bIsWithinComboWindow;

	UPROPERTY()
	bool bReceivedInputAtRightTime;

	UFUNCTION()
	void HandleComboDetectStart(FGameplayEventData Payload);

	UFUNCTION()
	void HandleComboDetectEnd(FGameplayEventData Payload);

	UFUNCTION()
	void HandleComboInputDetect(FGameplayEventData Payload);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int ComboCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float BaseDamage;
};

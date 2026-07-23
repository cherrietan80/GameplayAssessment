// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/GA_MeleeAttack_Base.h"
#include "GA_MeleeAttack_AirCombo.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYASSESSMENT_API UGA_MeleeAttack_AirCombo : public UGA_MeleeAttack_Base
{
	GENERATED_BODY()

public:
	UGA_MeleeAttack_AirCombo();

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

private:
	UFUNCTION()
	void HandleKnockUp(FGameplayEventData Payload);

	UFUNCTION()
	void HandleKnockDown(FGameplayEventData Payload);

	UFUNCTION()
	void ApplyPlayerAirEffect();

	UFUNCTION()
	void RemoveAirEffect();

	UFUNCTION()
	void RemovePlayerAirEffect();

	UFUNCTION()
	void HandleAirTimeout();

private:
	FActiveGameplayEffectHandle PlayerAirHandle;

	FActiveGameplayEffectHandle EnemyAirHandle;

	AActor* TargetEnemy;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AirState")
	TSubclassOf<UGameplayEffect> AirEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AirState")
	float FollowDelay = 0.2f;
};

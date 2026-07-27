// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

UCLASS()
class GAMEPLAYASSESSMENT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void HandleDeath() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	void DestroyActor();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void CreateHealthWidget();

	UFUNCTION(BlueprintCallable)
	void ApplyKnockback(const FVector& Direction, float Strength);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* HealthBarComponent;

private:
	FTimerHandle AttackTimer;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeath OnEnemyDeath;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/WeaponData.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "BaseWeapon.generated.h"

UCLASS()
class GAMEPLAYASSESSMENT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FWeaponConfig GetWeaponConfig() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitScan")
	USceneComponent* TraceStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitScan")
	USceneComponent* TraceEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FWeaponConfig WeaponConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitScan")
	float HitScanRadius;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "HitScan")
	void HitActor();

	UFUNCTION(BlueprintCallable, Category = "HitScan")
	void StartHitActor(FGameplayEventData Payload);

	UFUNCTION(BlueprintCallable, Category = "HitScan")
	void EndHitActor(FGameplayEventData Payload);

private:
	FTimerHandle HitScanTimer;

};

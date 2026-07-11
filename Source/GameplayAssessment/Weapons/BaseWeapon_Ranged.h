// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "BaseWeapon_Ranged.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int, NewAmmo);

UCLASS()
class GAMEPLAYASSESSMENT_API ABaseWeapon_Ranged : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ABaseWeapon_Ranged();

	UFUNCTION(BlueprintCallable, Category = "Ranged")
	void Fire();

	UFUNCTION(Blueprintpure, Category = "Damage")
	float GetDamageAmount() const { return DamageAmount; }

	UFUNCTION(Blueprintpure, Category = "Ranged")
	int GetAmmoAmount() const { return Ammo; }

	UFUNCTION(Blueprintpure, Category = "Ranged")
	int GetAmmoPerMagazineAmount() const { return AmmoPerMagazine; }

	UFUNCTION(BlueprintCallable, Category = "Ranged")
	void FullFillAmmo();

	UFUNCTION(BlueprintCallable, Category = "LineTrace")
	void CalculateStartAndEndPoint(AActor* OwnerActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	int Ammo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	int AmmoPerMagazine = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageAmount = -25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	float TraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	FVector StartPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	FVector EndPoint;

public:
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChanged OnAmmoChanged;

};

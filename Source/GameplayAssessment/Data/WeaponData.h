// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponConfig
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    TArray<TSubclassOf<class UGameplayAbility>> Abilities;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    FName SocketName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UAnimInstance> AnimBlueprint;
};
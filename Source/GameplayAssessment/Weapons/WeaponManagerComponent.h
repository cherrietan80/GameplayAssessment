// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "WeaponManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYASSESSMENT_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<class ABaseWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipWeapon(TSubclassOf<class ABaseWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ToggleEquipWeapon(TSubclassOf<class ABaseWeapon> WeaponClass);

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class ABaseWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<class UAnimInstance> DefaultAnimBlueprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

private:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;

	


		
};

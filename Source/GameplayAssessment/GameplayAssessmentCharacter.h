// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BaseCharacter.h"
#include "Logging/LogMacros.h"
#include <Weapons/WeaponManagerComponent.h>
#include "GameplayAssessmentCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AGameplayAssessmentCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeleeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* WandAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeleeSwingAttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponManagerComponent* WeaponManagerComponent;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

private:
	FTimerHandle StaminaRegenDelayTimer;

public:

	/** Constructor */
	AGameplayAssessmentCharacter();	

private:
	void OnStaminaChanged(const FOnAttributeChangeData& Data);

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoDash();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoMeleeAttackSwing();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ToggleEquipMelee();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ToggleEquipWand();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void StartStaminaRegen();

public:
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<class UGameplayEffect> StaminaRegenEffect;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BaseCharacter.h"
#include "Logging/LogMacros.h"
#include "GameplayAssessmentCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboCountChanged, int, ComboCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCriticalHit);

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
	UInputAction* RangedAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeleeSwingAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeleeComboAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeleeOmniAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MeleeAirComboAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RangedShootAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RangedAimAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RangedReloadAction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AimedMaxSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float NormalMaxSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "CameraBoom")
	float MinLength = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "CameraBoom")
	float MaxLength = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "CameraBoom")
	FVector AimedSocketOffset = FVector(0.f, 50.f, 50.f);

	UPROPERTY(EditDefaultsOnly, Category = "AttackCombo")
	int ComboCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "AttackCombo")
	float ComboResetTime = 3.f;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

private:
	FTimerHandle StaminaRegenDelayTimer;
	FTimerHandle ComboResetHandle;
	float TargetLength;
	FVector TargetSocketOffset;

public:

	/** Constructor */
	AGameplayAssessmentCharacter();	
	virtual void Tick(float DeltaTime) override;

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

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoMeleeAttackCombo();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoMeleeAttackOmni();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoMeleeAttackAirCombo();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoRangedShooting();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoRangedAim();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void CancelRangedAim();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void DoRangedReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ToggleEquipMelee();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ToggleEquipRanged();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void StartStaminaRegen();

	UFUNCTION(BlueprintCallable, Category = "Ranged")
	void EnterAimMode();

	UFUNCTION(BlueprintCallable, Category = "Ranged")
	void ExitAimMode();

	UFUNCTION(BlueprintCallable, Category = "AttackCombo")
	void RefreshComboTimer();

	UFUNCTION(BlueprintCallable, Category = "AttackCombo")
	void ResetComboCount();

	UFUNCTION(BlueprintCallable, Category = "AttackCombo")
	void RegisterCombo();

public:
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<class UGameplayEffect> StaminaRegenEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ranged")
	bool bIsAiming = false;

	UPROPERTY(BlueprintAssignable)
	FOnComboCountChanged OnComboCountChanged;

	UPROPERTY(BlueprintAssignable, Category = "CriticalHit")
	FOnCriticalHit OnCriticalHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CriticalHit")
	bool bIsCritical = false;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

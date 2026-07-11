// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_MeleeAttack_Combo.h"
#include "GameAbilitiesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_MeleeAttack_Combo::UGA_MeleeAttack_Combo()
{
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_Combo);
	BaseDamage = DamageAmount;
}

void UGA_MeleeAttack_Combo::MontageStart()
{
	ComboCount = 1;

	// Now UAbilityTask_WaitGameplayEvent is recognized
	UAbilityTask_WaitGameplayEvent* ContinueComboDetecBeginTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_GameplayEvent_ContinueCombo_Start
	);

	ContinueComboDetecBeginTask->EventReceived.AddDynamic(
		this,
		&UGA_MeleeAttack_Combo::HandleComboDetectStart
	);

	ContinueComboDetecBeginTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* ContinueComboDetecEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_GameplayEvent_ContinueCombo_End
	);

	ContinueComboDetecEndTask->EventReceived.AddDynamic(
		this,
		&UGA_MeleeAttack_Combo::HandleComboDetectEnd
	);

	ContinueComboDetecEndTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* ComboInputDetect = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_GameplayEvent_ContinueCombo_Input
	);

	ComboInputDetect->EventReceived.AddDynamic(
		this,
		&UGA_MeleeAttack_Combo::HandleComboInputDetect
	);

	ComboInputDetect->ReadyForActivation();
}

void UGA_MeleeAttack_Combo::CalculateDamageAmount()
{
	DamageAmount = BaseDamage;

	if (ComboCount == 3)
	{
		DamageAmount = 2 * BaseDamage;
	}
}

void UGA_MeleeAttack_Combo::HandleComboDetectStart(FGameplayEventData Payload)
{
	bIsWithinComboWindow = true;
	bReceivedInputAtRightTime = false;
}

void UGA_MeleeAttack_Combo::HandleComboDetectEnd(FGameplayEventData Payload)
{
	bIsWithinComboWindow = false;
	if (!bReceivedInputAtRightTime)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
	else
	{
		ComboCount++;
	}
}

void UGA_MeleeAttack_Combo::HandleComboInputDetect(FGameplayEventData Payload)
{
	bReceivedInputAtRightTime = bIsWithinComboWindow;
}



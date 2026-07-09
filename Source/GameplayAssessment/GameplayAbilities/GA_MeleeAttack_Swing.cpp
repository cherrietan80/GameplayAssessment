// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_MeleeAttack_Swing.h"
#include "GameAbilitiesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon.h"

UGA_MeleeAttack_Swing::UGA_MeleeAttack_Swing()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_Swing);
	bShouldShowInAbilityBar = true;
}

void UGA_MeleeAttack_Swing::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWeaponManagerComponent* WeaponManagerComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UWeaponManagerComponent>();

	if (!WeaponManagerComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	ABaseWeapon* EquippedWeapon = WeaponManagerComponent->GetEquippedWeapon();

	if (!EquippedWeapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if(MeleeAttackSwingMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,                 
			NAME_None,            
			MeleeAttackSwingMontage
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack_Swing::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack_Swing::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_MeleeAttack_Swing::OnMontageCancelled);

		MontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* HitScanBeginTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			TAG_GameplayEvent_HitScan_Start
			);

		if (EquippedWeapon)
		{
			HitScanBeginTask->EventReceived.AddDynamic(
				EquippedWeapon,
				&ABaseWeapon::StartHitActor
			);

			UE_LOG(LogTemp, Display, TEXT("gogoinside"));
		}

		HitScanBeginTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* HitScanEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			TAG_GameplayEvent_HitScan_End
		);

		if (EquippedWeapon)
		{
			HitScanEndTask->EventReceived.AddDynamic(
				EquippedWeapon,
				&ABaseWeapon::EndHitActor
			);
		}

		HitScanEndTask->ReadyForActivation();
	}

	
}

void UGA_MeleeAttack_Swing::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_MeleeAttack_Swing::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_MeleeAttack_Swing::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

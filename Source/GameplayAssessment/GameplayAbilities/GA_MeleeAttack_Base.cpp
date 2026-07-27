// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_MeleeAttack_Base.h"
#include "GameAbilitiesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon.h"
#include "AbilitySystemComponent.h"

UGA_MeleeAttack_Base::UGA_MeleeAttack_Base()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_Base);
	bShouldShowInAbilityBar = true;
}

void UGA_MeleeAttack_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWeaponManagerComponent* WeaponManagerComponent = GetAvatarActorFromActorInfo()->GetComponentByClass<UWeaponManagerComponent>();

	if (!WeaponManagerComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	EquippedWeapon = WeaponManagerComponent->GetEquippedWeapon();

	if (!EquippedWeapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if(MeleeAttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,                 
			NAME_None,            
			MeleeAttackMontage
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack_Base::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack_Base::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_MeleeAttack_Base::OnMontageCancelled);

		MontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* HitScanBeginTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			TAG_GameplayEvent_HitScan_Start
			);

		HitScanBeginTask->EventReceived.AddDynamic(
			this,
			&UGA_MeleeAttack_Base::HandleHitScanStart
		);

		HitScanBeginTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* HitScanEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			TAG_GameplayEvent_HitScan_End
		);

		HitScanEndTask->EventReceived.AddDynamic(
			this,
			&UGA_MeleeAttack_Base::HandleHitScanEnd
		);

		HitScanEndTask->ReadyForActivation();

		MontageStart();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
	
}

void UGA_MeleeAttack_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

	if (EquippedWeapon)
	{
		EquippedWeapon->EndHitActor();
	}
}

void UGA_MeleeAttack_Base::HandleHitScanStart(FGameplayEventData Payload)
{
	CalculateDamageAmount();

	if (EquippedWeapon)
	{
		UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DamageEffectClass,
			1.f,
			Context);

		SpecHandle.Data->SetSetByCallerMagnitude(
			TAG_Data_Damage,
			DamageAmount
		);

		EquippedWeapon->EffectSpec = *SpecHandle.Data.Get();

		EquippedWeapon->StartHitActor();
	}
}

void UGA_MeleeAttack_Base::HandleHitScanEnd(FGameplayEventData Payload)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->EndHitActor();
	}
}

void UGA_MeleeAttack_Base::MontageStart()
{
}

void UGA_MeleeAttack_Base::CalculateDamageAmount()
{
	if (EquippedWeapon)
	{
		DamageAmount = BaseDamageAmount;
		if (EquippedWeapon->CurrentHitEventTag == TAG_GameplayEvent_Hit_Strong || EquippedWeapon->CurrentHitEventTag == TAG_GameplayEvent_Hit_Down)
		{
			DamageAmount = BaseDamageAmount * 2;
		}
	}
}

void UGA_MeleeAttack_Base::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_MeleeAttack_Base::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_MeleeAttack_Base::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}


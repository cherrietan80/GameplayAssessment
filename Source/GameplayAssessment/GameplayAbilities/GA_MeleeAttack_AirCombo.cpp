// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_MeleeAttack_AirCombo.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"

UGA_MeleeAttack_AirCombo::UGA_MeleeAttack_AirCombo()
{
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_AirCombo);
	ActivationOwnedTags.AddTag(TAG_Ability_MeleeAttack_AirCombo_Active);
	ActiveUITag = TAG_Ability_MeleeAttack_AirCombo_Active;
	CancelAbilitiesWithTag.AddTag(TAG_Ability_MeleeAttack_Swing);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_MeleeAttack_Combo);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_MeleeAttack_Swing);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_MeleeAttack_Combo);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_HitReaction);
}

void UGA_MeleeAttack_AirCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilityTask_WaitGameplayEvent* KnockUpTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_GameplayEvent_Hit_Air
	);

	KnockUpTask->EventReceived.AddDynamic(
		this,
		&UGA_MeleeAttack_AirCombo::HandleKnockUp
	);

	KnockUpTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* KnockDownTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_GameplayEvent_Hit_Down
	);

	KnockDownTask->EventReceived.AddDynamic(
		this,
		&UGA_MeleeAttack_AirCombo::HandleKnockDown
	);

	KnockDownTask->ReadyForActivation();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitDelay* TimeoutTask = UAbilityTask_WaitDelay::WaitDelay(this, 1.f);

	TimeoutTask->OnFinish.AddDynamic(
		this,
		&UGA_MeleeAttack_AirCombo::HandleAirTimeout
	);

	TimeoutTask->ReadyForActivation();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
	{
		PC->SetIgnoreMoveInput(true);
	}
}

void UGA_MeleeAttack_AirCombo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveAirEffect();
	RemovePlayerAirEffect();
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
	{
		PC->SetIgnoreMoveInput(false);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_MeleeAttack_AirCombo::HandleKnockUp(FGameplayEventData Payload)
{
	TargetEnemy = const_cast<AActor*>(Payload.Target.Get());

	if (!TargetEnemy)
	{
		return;
	}

	UAbilitySystemComponent* EnemyASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			TargetEnemy
		);

	if (!EnemyASC)
	{
		return;
	}

	ABaseCharacter* EnemyChar = Cast<ABaseCharacter>(TargetEnemy);
	ABaseCharacter* PlayerChar = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());

	if (!EnemyChar or !PlayerChar)
	{
		return;
	}
	
	EnemyChar->AirComboHeight = EnemyChar->GetActorLocation().Z + 400.f;
	PlayerChar->AirComboHeight = EnemyChar->GetActorLocation().Z + 400.f;

	FGameplayEffectContextHandle Context = EnemyASC->MakeEffectContext();

	FGameplayEffectSpecHandle Spec = EnemyASC->MakeOutgoingSpec(
			AirEffectClass,
			1.f,
			Context
		);

	EnemyAirHandle = EnemyASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

	// Delay 0.3s before player goes up
	UAbilityTask_WaitDelay* PlayerDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.3f);

	PlayerDelayTask->OnFinish.AddDynamic(this, &UGA_MeleeAttack_AirCombo::ApplyPlayerAirEffect);

	PlayerDelayTask->ReadyForActivation();
}

void UGA_MeleeAttack_AirCombo::HandleKnockDown(FGameplayEventData Payload)
{
	RemoveAirEffect();
}

void UGA_MeleeAttack_AirCombo::ApplyPlayerAirEffect()
{
	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();

	if (!PlayerASC)
	{
		return;
	}

	FGameplayEffectContextHandle Context = PlayerASC->MakeEffectContext();

	FGameplayEffectSpecHandle Spec = PlayerASC->MakeOutgoingSpec(
			AirEffectClass,
			1.f,
			Context
		);

	PlayerAirHandle = PlayerASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UGA_MeleeAttack_AirCombo::RemoveAirEffect()
{
	if (TargetEnemy)
	{
		UAbilitySystemComponent* EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetEnemy);

		if (EnemyASC && EnemyAirHandle.IsValid())
		{
			EnemyASC->RemoveActiveGameplayEffect(EnemyAirHandle);
		}
	}
}

void UGA_MeleeAttack_AirCombo::RemovePlayerAirEffect()
{
	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();

	if (PlayerASC && PlayerAirHandle.IsValid())
	{
		PlayerASC->RemoveActiveGameplayEffect(PlayerAirHandle);
	}
}

void UGA_MeleeAttack_AirCombo::HandleAirTimeout()
{
	if (TargetEnemy)
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}



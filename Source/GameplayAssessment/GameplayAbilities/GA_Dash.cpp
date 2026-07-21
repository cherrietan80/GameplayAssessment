// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AttributeSets/BasicAttributeSet.h"
#include "BaseCharacter.h"

UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(TAG_Ability_Dash);
	bShouldShowInAbilityBar = true;

	CancelAbilitiesWithTag.AddTag(TAG_Ability_Base);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_Base);
	ActivationOwnedTags.AddTag(TAG_Ability_Dash_Active);
	ActiveUITag = TAG_Ability_Dash_Active;
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (!MoveComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CommitAbilityCost(Handle, ActorInfo, ActivationInfo);

	UAbilityTask_ApplyRootMotionConstantForce* Task =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,
			NAME_None,
			GetDashDirection(),
			Strength,
			Duration,
			false,
			nullptr,
			ERootMotionFinishVelocityMode::ClampVelocity,
			FVector::ZeroVector,
			GetMaxSpeed(),
			true
		);

	K2_AddGameplayCue(TAG_GameplayCue_Dash, FGameplayEffectContextHandle());

	Task->OnFinish.AddDynamic(this, &UGA_Dash::OnDashFinished);

	Task->ReadyForActivation();
}

void UGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->ActivateAbilityByTag(TAG_Ability_MeleeAttack_Swing);
	}
}

FVector UGA_Dash::GetDashDirection() const
{
	APawn* CharacterPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	if (CharacterPawn)
	{
		FVector LastVector = CharacterPawn->GetLastMovementInputVector();

		if(!LastVector.IsZero())
		{
			return LastVector.GetSafeNormal();
		}
	}
	return GetAvatarActorFromActorInfo()->GetActorForwardVector();
}

float UGA_Dash::GetMaxSpeed() const
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
		if (MoveComp)
		{
			return MoveComp->GetMaxSpeed();
		}
	}
	return 500.0f;
}

void UGA_Dash::OnDashFinished()
{
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, nullptr);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

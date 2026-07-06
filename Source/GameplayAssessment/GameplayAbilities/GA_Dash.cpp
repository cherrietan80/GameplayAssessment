// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AttributeSets/BasicAttributeSet.h"

UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(TAG_Ability_Dash);
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
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

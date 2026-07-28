// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_AirLaunch.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "BaseCharacter.h"
#include "GameAbilitiesGameplayTags.h"

UGA_AirLaunch::UGA_AirLaunch()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	AbilityTags.AddTag(TAG_Ability_AirLaunch);
	bShouldShowInAbilityBar = false;
	ActivationOwnedTags.AddTag(TAG_Ability_AirLaunch_Active);
	ActiveUITag = TAG_Ability_AirLaunch_Active;
}

void UGA_AirLaunch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FVector Target = Character->GetActorLocation();

	Target.Z = Character->AirComboHeight;

	UAbilityTask_ApplyRootMotionMoveToForce* MoveTask = UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce
	(
		this,
		NAME_None,
		Target,
		LaunchDuration,
		true,           
		MOVE_Flying,          
		true,                
		nullptr,          
		ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
		FVector::ZeroVector,
		0.f
	);
	MoveTask->OnTimedOutAndDestinationReached.AddDynamic(this, &UGA_AirLaunch::FinishLaunch);
	MoveTask->ReadyForActivation();
}

void UGA_AirLaunch::FinishLaunch()
{
	UE_LOG(LogTemp, Warning, TEXT("AirLaunch End"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

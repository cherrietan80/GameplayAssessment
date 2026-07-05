// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"

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

	TSharedPtr<FRootMotionSource_ConstantForce> Force = MakeShared<FRootMotionSource_ConstantForce>();

	Force->Priority = 5;
	Force->Duration = 0.2f;
	Force->Force = GetDashDirection() * Strength;
	Force->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::ClampVelocity;
	Force->FinishVelocityParams.ClampVelocity = GetMaxSpeed();
	Force->AccumulateMode = ERootMotionAccumulateMode::Additive;
	MoveComp->ApplyRootMotionSource(Force);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
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

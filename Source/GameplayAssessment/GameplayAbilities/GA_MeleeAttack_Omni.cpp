// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_MeleeAttack_Omni.h"
#include "GameFramework/Character.h"
#include "GameAbilitiesGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_MeleeAttack_Omni::UGA_MeleeAttack_Omni()
{
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_Omni);
	ActivationOwnedTags.AddTag(TAG_Ability_MeleeAttack_Omni_Active);
	ActiveUITag = TAG_Ability_MeleeAttack_Omni_Active;

	BlockAbilitiesWithTag.AddTag(TAG_Ability_Dash);
	BlockAbilitiesWithTag.AddTag(TAG_Ability_HitReaction);
	CancelAbilitiesWithTag.AddTag(TAG_Ability_HitReaction);
}

void UGA_MeleeAttack_Omni::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

	if (!Movement)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	Movement->DisableMovement();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_MeleeAttack_Omni::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
		if (Movement)
		{
			Movement->SetMovementMode(MOVE_Walking);
		}
	}
}

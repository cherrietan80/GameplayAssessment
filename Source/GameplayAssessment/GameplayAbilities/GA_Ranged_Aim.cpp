// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GA_Ranged_Aim.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "GameplayAssessmentCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

UGA_Ranged_Aim::UGA_Ranged_Aim()
{
	AbilityTags.AddTag(TAG_Ability_Ranged_Aim);
	bShouldShowInAbilityBar = true;
}

void UGA_Ranged_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AGameplayAssessmentCharacter* Character = Cast<AGameplayAssessmentCharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	Character->EnterAimMode();

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_Aim_Start;

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);

	if (!AbilitySystemComponent)
		return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Character,
		Data.EventTag,
		Data
	);
}

void UGA_Ranged_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AGameplayAssessmentCharacter* Character = Cast<AGameplayAssessmentCharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		return;
	}

	Character->ExitAimMode();

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_Aim_End;

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);

	if (!AbilitySystemComponent)
		return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Character,
		Data.EventTag,
		Data
	);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Ranged_Aim::OnMontageInterrupted()
{

}

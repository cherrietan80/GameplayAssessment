// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/BaseGameplayAbility.h"
#include "GameAbilitiesGameplayTags.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	ActivationBlockedTags.AddTag(TAG_State_Death);
	ActivationOwnedTags.AddTag(TAG_Ability_Active);
}

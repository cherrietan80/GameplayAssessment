// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ANState_ComboWindow.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "AbilitySystemGlobals.h"

FString UANState_ComboWindow::GetNotifyName_Implementation() const
{
	return TEXT("Continue Combo Window");
}

void UANState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_ContinueCombo_Start;

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());

	if (!AbilitySystemComponent)
		return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		Data.EventTag,
		Data
	);
}

void UANState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_ContinueCombo_End;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		Data.EventTag,
		Data
	);
}

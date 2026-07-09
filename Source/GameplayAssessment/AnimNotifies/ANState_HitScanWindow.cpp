#include "ANState_HitScanWindow.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include <AbilitySystemGlobals.h>

FString UANState_HitScanWindow::GetNotifyName_Implementation() const
{
	return TEXT("Hit Scan Window");
}

void UANState_HitScanWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_HitScan_Start;

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());

	UE_LOG(LogTemp, Warning, TEXT("Owner: %s"),
		MeshComp->GetOwner() ? *MeshComp->GetOwner()->GetName() : TEXT("None"));

	if (!AbilitySystemComponent)
		return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		Data.EventTag,
		Data
	);
}

void UANState_HitScanWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UANState_HitScanWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_HitScan_End;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		Data.EventTag,
		Data
	);
}

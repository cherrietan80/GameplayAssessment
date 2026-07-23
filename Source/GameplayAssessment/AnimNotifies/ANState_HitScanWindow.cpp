#include "ANState_HitScanWindow.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"
#include "AbilitySystemGlobals.h"
#include "Weapons/WeaponManagerComponent.h"
#include "Weapons/BaseWeapon.h"

FString UANState_HitScanWindow::GetNotifyName_Implementation() const
{
	return TEXT("Hit Scan Window");
}

void UANState_HitScanWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UWeaponManagerComponent* WeaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponManagerComponent>();
	if (WeaponComponent)
	{
		WeaponComponent->GetEquippedWeapon()->CurrentHitEventTag = HitEventTag;
	}

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_HitScan_Start;

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());

	if (!AbilitySystemComponent)
		return;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		Data.EventTag,
		Data
	);
}

void UANState_HitScanWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UWeaponManagerComponent* WeaponComponent = MeshComp->GetOwner()->GetComponentByClass<UWeaponManagerComponent>();
	if (WeaponComponent)
	{
		WeaponComponent->GetEquippedWeapon()->CurrentHitEventTag = TAG_Ability_HitReaction;
	}

	FGameplayEventData Data;
	Data.EventTag = TAG_GameplayEvent_HitScan_End;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		Data.EventTag,
		Data
	);
}

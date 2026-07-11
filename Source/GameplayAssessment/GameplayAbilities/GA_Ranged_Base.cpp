
#include "GA_Ranged_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameAbilitiesGameplayTags.h"

UGA_Ranged_Base::UGA_Ranged_Base()
{
	AbilityTags.AddTag(TAG_Ability_Ranged_Base);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Ranged_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!ActionMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
		return;
	}
		
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ActionMontage
	);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_Ranged_Base::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Ranged_Base::OnMontageCancelled);

	MontageTask->ReadyForActivation();
}

void UGA_Ranged_Base::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Ranged_Base::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

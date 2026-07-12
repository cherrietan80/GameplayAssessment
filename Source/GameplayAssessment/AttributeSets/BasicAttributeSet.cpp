// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSets/BasicAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayAbilities/GameAbilitiesGameplayTags.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	Stamina = 100.0f;
	MaxStamina = 100.0f;
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());

		if (Data.EffectSpec.Def)
		{
			if (Data.EffectSpec.Def->GetAssetTags().HasTag(TAG_GameplayEffect_HitReaction))
			{
				FGameplayTagContainer Container;
				Container.AddTag(TAG_Ability_HitReaction);
				GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(Container);
			}
		}
	}
	else if(Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
}

void UBasicAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute() && NewValue <= 0.f)
	{
		FGameplayTagContainer Container;
		Container.AddTag(TAG_Ability_Death);
		GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(Container);
	}
}

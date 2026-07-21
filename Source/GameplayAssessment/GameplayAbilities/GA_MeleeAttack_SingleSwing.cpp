
#include "GA_MeleeAttack_SingleSwing.h"
#include "GameAbilitiesGameplayTags.h"

UGA_MeleeAttack_SingleSwing::UGA_MeleeAttack_SingleSwing()
{
	AbilityTags.AddTag(TAG_Ability_MeleeAttack_Swing);
	ActivationOwnedTags.AddTag(TAG_Ability_MeleeAttack_Swing_Active);
	ActiveUITag = TAG_Ability_MeleeAttack_Swing_Active;
}

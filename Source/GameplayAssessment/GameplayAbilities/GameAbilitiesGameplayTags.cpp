#include "GameplayAbilities/GameAbilitiesGameplayTags.h"

//Gameplay Abilities Gameplay Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Dash, "GameplayAbility.Movement.Dash");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Swing, "GameplayAbility.MeleeAttack.Swing");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Combo, "GameplayAbility.MeleeAttack.Combo");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Base, "GameplayAbility.MeleeAttack");

//Gameplay Cues Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Dash, "GameplayCue.Dash");

//Gameplay Effects Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEffect_StaminaRegen, "Status.Stamina.Regen");

//Gameplay Events Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_AbilityChanged, "Event.Abilities.Changed");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_Weapon_Equip, "Event.Weapon.Equip");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_HitScan_Start, "Event.HitScan.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_HitScan_End, "Event.HitScan.End");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_ContinueCombo_Start, "Event.ContinuCombo.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_ContinueCombo_End, "Event.ContinuCombo.End");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_ContinueCombo_Input, "Event.ContinuCombo.Input");

//Weapon Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Melee, "Weapon.Melee");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Wand, "Weapon.Wand");

//Data Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Damage, "Data.Damage");

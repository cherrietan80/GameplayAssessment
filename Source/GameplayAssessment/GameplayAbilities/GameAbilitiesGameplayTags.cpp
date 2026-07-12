#include "GameplayAbilities/GameAbilitiesGameplayTags.h"

//Gameplay Abilities Gameplay Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Dash, "GameplayAbility.Movement.Dash");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Base, "GameplayAbility");
//Melee
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Swing, "GameplayAbility.MeleeAttack.Swing");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Combo, "GameplayAbility.MeleeAttack.Combo");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Base, "GameplayAbility.MeleeAttack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_MeleeAttack_Sweep, "GameplayAbility.MeleeAttack.Sweep");
//Ranged
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Ranged_Base, "GameplayAbility.Ranged")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Ranged_Shoot, "GameplayAbility.Ranged.Shoot")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Ranged_Aim, "GameplayAbility.Ranged.Aim")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Ranged_Reload, "GameplayAbility.Ranged.Reload")
//HitReaction
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_HitReaction, "GameplayAbility.HitReaction");
//Die
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Death, "GameplayAbility.Death");

//Gameplay Cues Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Dash, "GameplayCue.Dash");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Damage, "GameplayCue.Damage.Burst");

//Gameplay Effects Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEffect_StaminaRegen, "Status.Stamina.Regen");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEffect_HitReaction, "Effect.HitReaction");

//Gameplay Events Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_AbilityChanged, "Event.Abilities.Changed");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_Weapon_Equip, "Event.Weapon.Equip");
//HitScan
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_HitScan_Start, "Event.HitScan.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_HitScan_End, "Event.HitScan.End");
//MeleeCombo
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_ContinueCombo_Start, "Event.ContinuCombo.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_ContinueCombo_End, "Event.ContinuCombo.End");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_ContinueCombo_Input, "Event.ContinuCombo.Input");
//Aim
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_Aim_Start, "Event.Aim.Start");
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayEvent_Aim_End, "Event.Aim.End");

//Weapon Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Melee, "Weapon.Melee");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Ranged, "Weapon.Ranged");

//Data Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Damage, "Data.Damage");

//State Tags
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Death, "State.Death");

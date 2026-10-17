#pragma once

#include "NativeGameplayTags.h"

#pragma region Attributes
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Meta_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Meta_Heal);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Health);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_MaxHealth);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Energy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_MaxEnergy);
#pragma endregion Attributes

#pragma region Events
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_HitRegistered);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_DamageTaken);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Heal_Started);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Heal_Ended);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Heal_Received);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Heal_Interrupted);
#pragma endregion Events

#pragma region States
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Healing);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Staggered);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Dashing);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Invulnerable);
#pragma endregion States

#pragma region Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Combat_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Combat_Heal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Innate_Dash);
#pragma endregion Abilities

#pragma region Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Heal);
#pragma endregion Abilities

#pragma region Progression
#pragma region InnateAbilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Progression_InnateAbility_Dash);
#pragma endregion InnateAbilities
#pragma region Tools
// This is for later
#pragma endregion Tools
#pragma endregion Progression



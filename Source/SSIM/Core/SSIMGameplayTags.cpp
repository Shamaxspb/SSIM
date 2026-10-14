#include "SSIMGameplayTags.h"

#pragma region Attributes
/* Meta Attributes */
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Meta_Damage, 		  "Attribute.Meta.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Meta_Heal,	  		  "Attribute.Meta.Heal");

/* Stat Attributes */
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Health,	"Attribute.Health");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_MaxHealth,	"Attribute.MaxHealth");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_Energy,	"Attribute.Energy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attribute_MaxEnergy, "Attribute.MaxEnergy");
#pragma endregion Attributes

/* Events */
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_HitRegistered,	"Event.HitRegistered");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_DamageTaken,	"Event.DamageTaken");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_HealReceived,	"Event.HealReceived");

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Started,	"Event.Heal.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Ended,	"Event.Heal.Ended");

/* State */
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Healing,	   "State.Healing");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Staggered,	   "State.Staggered");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Dashing,	   "State.Dashing");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Invulnerable, "State.Invulnerable");

/* Abilities */
// Basic
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Combat_Attack, "Ability.Combat.Attack");
// Innate
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Combat_Heal,   "Ability.Innate.Heal");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Innate_Dash,   "Ability.Innate.Dash");

/* Gameplay Cues */
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Heal, "GameplayCue.Heal");

/* PROGRESSION */
/* Innate Abilities */
UE_DEFINE_GAMEPLAY_TAG(TAG_Progression_InnateAbility_Dash, "Progression.InnateAbility.Dash");


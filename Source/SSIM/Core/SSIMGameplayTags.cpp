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
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_HitRegistered,		"Event.HitRegistered");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_DamageTaken,		"Event.DamageTaken");

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Attack_Started,		"Event.Attack.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Attack_Ended,			"Event.Attack.Ended");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Attack_Interrupted,	"Event.Attack.Interrupted");

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Pogo_Started,				"Event.Pogo.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Pogo_Ended,				"Event.Pogo.Ended");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Pogo_Animation_Started,	"Event.Pogo.Animation.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Pogo_Animation_Ended,		"Event.Pogo.Animation.Ended");

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Started,		"Event.Heal.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Ended,		"Event.Heal.Ended");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Received,		"Event.Heal.Received");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Interrupted,	"Event.Heal.Interrupted");

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Dash_Started,		"Event.Dash.Started");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Dash_Ended,		"Event.Dash.Ended");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Dash_Interrupted,	"Event.Dash.Interrupted");

/* State */
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Attacking,	   "State.Attacking");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Healing,	   "State.Healing");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Staggered,	   "State.Staggered");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Dashing,	   "State.Dashing");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Invulnerable, "State.Invulnerable");

UE_DEFINE_GAMEPLAY_TAG(TAG_State_Modifier_MovementBlock, "State.Modifier.MovementBlock");

/* Abilities */
// Basic
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Combat_Attack_Frontal,	"Ability.Combat.Attack.Frontal");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Combat_Attack_Upward,	"Ability.Combat.Attack.Upward");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Combat_Attack_Pogo,		"Ability.Combat.Attack.Pogo");

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Combat_Heal,				"Ability.Combat.Heal");
// Innate
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Innate_Dash,				"Ability.Innate.Dash");

/* Gameplay Cues */
UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Heal, "GameplayCue.Heal");

/* PROGRESSION */
/* Innate Abilities */
UE_DEFINE_GAMEPLAY_TAG(TAG_Progression_InnateAbility_Dash, "Progression.InnateAbility.Dash");


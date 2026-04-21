// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSSIMInheritance,			 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMPlayerInitialization, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMEnemyInitialization,  Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMUIInitialization,	 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMGameplayMessages,	 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMValidations,			 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMStatsCalculation,	 Log, All);

constexpr ECollisionChannel ECC_Player			= ECC_GameTraceChannel1;
constexpr ECollisionChannel ECC_Enemy			= ECC_GameTraceChannel2;
constexpr ECollisionChannel ECC_MeleeAttack		= ECC_GameTraceChannel3;
constexpr ECollisionChannel ECC_Projectile		= ECC_GameTraceChannel4;
constexpr ECollisionChannel ECC_HitRegistration	= ECC_GameTraceChannel5;
constexpr ECollisionChannel ECC_ContactDamage	= ECC_GameTraceChannel6;

// Globals
/*const*/ inline float DEFAULT_GRAVITY_SCALE = 7.f;
/*const*/ inline float DEFAULT_BRAKING_DECELERATION_WALKING = 10000.f;

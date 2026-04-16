// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSSIMInheritance,			 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMPlayerInitialization, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMEnemyInitialization,  Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMGameplayMessages,	 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMValidations,			 Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSSIMStatsCalculation,	 Log, All);

constexpr ECollisionChannel ECC_Player		= ECC_GameTraceChannel1;
constexpr ECollisionChannel ECC_AttackTrace = ECC_GameTraceChannel2;
constexpr ECollisionChannel ECC_Enemy		= ECC_GameTraceChannel3;

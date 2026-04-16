// Copyright Epic Games, Inc. All Rights Reserved.

#include "SSIM.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogSSIMInheritance);
DEFINE_LOG_CATEGORY(LogSSIMPlayerInitialization);
DEFINE_LOG_CATEGORY(LogSSIMEnemyInitialization);
DEFINE_LOG_CATEGORY(LogSSIMGameplayMessages);
DEFINE_LOG_CATEGORY(LogSSIMValidations);
DEFINE_LOG_CATEGORY(LogSSIMStatsCalculation);

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, SSIM, "SSIM" );

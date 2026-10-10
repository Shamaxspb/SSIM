// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMCombatSubsystemSettings.h"

FName USSIMCombatSubsystemSettings::GetCategoryName() const
{
	return TEXT("Game");
}

FText USSIMCombatSubsystemSettings::GetSectionText() const
{
	return NSLOCTEXT("Settings", "Combat", "SSIM Combat Subsystem Settings");
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMCombatSettings.h"

FName USSIMCombatSettings::GetCategoryName() const
{
	return TEXT("Game");
}

FText USSIMCombatSettings::GetSectionText() const
{
	return NSLOCTEXT("Settings", "Combat", "Combat Subsystem Settings");
}

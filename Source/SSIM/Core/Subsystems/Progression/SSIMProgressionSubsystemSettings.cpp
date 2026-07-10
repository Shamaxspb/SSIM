// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMProgressionSubsystemSettings.h"

FName USSIMProgressionSubsystemSettings::GetCategoryName() const
{
	return TEXT("Game");
}

#if WITH_EDITOR
FText USSIMProgressionSubsystemSettings::GetSectionText() const
{
	return NSLOCTEXT("Settings", "Progression", "Progression Subsystem Settings");
}
#endif WITH_EDITOR

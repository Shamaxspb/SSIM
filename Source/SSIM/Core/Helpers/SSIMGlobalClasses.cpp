// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMGlobalClasses.h"

FName USSIMGlobalClasses::GetCategoryName() const
{
	return TEXT("Game");
}

#if WITH_EDITOR
FText USSIMGlobalClasses::GetSectionText() const
{
	return NSLOCTEXT("Settings", "Gameplay", "SSIM Global Classes");
}
#endif WITH_EDITOR

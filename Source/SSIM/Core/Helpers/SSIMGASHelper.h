// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class UAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogSSIMGASHelper, Log, All);


namespace SSIMGASHelper
{
	SSIM_API void ApplyDamageToEnemy(AActor* InInstigator, AActor* InTarget, float InDamageAmount);
	
	SSIM_API void ApplyHealToPlayer(const UObject* WorldContextObject, int32 InHealAmount);
}

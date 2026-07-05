// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class UBlackboardComponent;

class SSIM_API USSIMGASHelper
{
public:
	static void ApplyGameplayEffectToSelf(UBlackboardComponent* BB, FName KeyName, uint8 Value);
	
};
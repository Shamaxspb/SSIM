// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class UBlackboardComponent;

class SSIM_API USSIMBlackboardHelper
{
public:
	static void SetEnumSafe(UBlackboardComponent* BB, FName KeyName, uint8 Value);
	static void SetFloatSafe(UBlackboardComponent* BB, FName KeyName, float Value);
	
private:
	static bool IsBlackboardAndKeyValid(const UBlackboardComponent* BB, FName KeyName);
};

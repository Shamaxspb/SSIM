// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class UBlackboardComponent;

class SSIM_API USSIMBlackboardHelper
{
public:
	static void SetEnumSafe(UBlackboardComponent* BB, FName KeyName, uint8 Value);
};

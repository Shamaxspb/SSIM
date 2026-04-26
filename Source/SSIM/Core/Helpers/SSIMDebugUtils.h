// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSIMDebugUtils.generated.h"

UCLASS()
class SSIM_API USSIMDebugUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG", meta = (WorldContext = "WorldContextObject"))
	static void PrintPlayerStates(const UObject* WorldContextObject);
	
};

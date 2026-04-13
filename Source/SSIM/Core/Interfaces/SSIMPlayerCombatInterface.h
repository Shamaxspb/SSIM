// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSIMPlayerCombatInterface.generated.h"

UINTERFACE()
class USSIMPlayerCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API ISSIMPlayerCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Combat")
	void ReceiveDamage(int32 InDamage) const;
};

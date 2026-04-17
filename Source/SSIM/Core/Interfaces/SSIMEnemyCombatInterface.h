// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSIMEnemyCombatInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USSIMEnemyCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API ISSIMEnemyCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Combat")
	void ReceiveDamageInterface(float InDamage) const;
	
};

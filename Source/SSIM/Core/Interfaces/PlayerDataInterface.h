// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerDataInterface.generated.h"

class USSIMPlayerCombatComponent;
class USSIMPlayerFlowComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerDataInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API IPlayerDataInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Components")
	USSIMPlayerCombatComponent* GetPlayerCombatComponentInterface() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Components")
	USSIMPlayerFlowComponent* GetPlayerFlowComponentInterface() const;
	
};

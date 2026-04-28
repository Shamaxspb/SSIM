// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "UObject/Interface.h"
#include "SSIMDamageableInterface.generated.h"


UINTERFACE()
class USSIMDamageableInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API ISSIMDamageableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Stats")
	void ReceiveDamageInterface(const FDamageData& InDamageData) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Stats")
	void ReceivePlayerAttackDirectionType(EPlayerAttackDirectionType InPlayerAttackDirectionType) const;
};

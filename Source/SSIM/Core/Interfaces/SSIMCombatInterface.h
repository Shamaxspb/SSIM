// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSIMCombatInterface.generated.h"

UINTERFACE()
class USSIMCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API ISSIMCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Combat")
	void StartAttackInterface() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Combat")
	void EndAttackInterface() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Attack")
	void StartAttackTraceInterface() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Attack")
	void EndAttackTraceInterface() const;
};

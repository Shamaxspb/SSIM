// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerDataInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UPlayerDataInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API IPlayerDataInterface
{
	GENERATED_BODY()

public:
	// This entire thing is just practice, and it is an overengineering, just get all this data in ActorComponent directly
	UFUNCTION(BlueprintCallable, Category = "SSIM|Animations")
	virtual UAnimMontage* GetDashAnimationInterface() = 0;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Animations")
	virtual TArray<UAnimMontage*> GetAttackAnimationsInterface() = 0;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Animations")
	virtual UAnimMontage* GetRandomAttackAnimationInterface() = 0;
	// ------------------ end of overengineering
};

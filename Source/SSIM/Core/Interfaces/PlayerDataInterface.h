// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerDataInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerDataInterface : public UInterface
{
	GENERATED_BODY()
};


class SSIM_API IPlayerDataInterface
{
	GENERATED_BODY()

public:
	// This entire thing is just practice, and it is an overengineering, just get all this data in ActorComponent directly
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Animations")
	TArray<UAnimMontage*> GetAttackAnimationsInterface() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SSIM|Animations")
	UAnimMontage* GetRandomAttackAnimationInterface() const;
	// ------------------ end of overengineering
};

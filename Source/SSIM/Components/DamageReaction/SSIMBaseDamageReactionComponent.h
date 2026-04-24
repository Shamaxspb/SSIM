// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"

#include "SSIMBaseDamageReactionComponent.generated.h"

struct FDamageData;
class USSIMBaseStatsComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseDamageReactionComponent : public USSIMBaseComponent
{
	GENERATED_BODY()

// Variables
public:
	
protected:
	UPROPERTY()
	TObjectPtr<USSIMBaseStatsComponent> BaseStatsComponent;

// Overriden Functions
protected:
	virtual void BeginPlay() override;

// My Functions
protected:
	virtual void SetReferences() override;
	
	UFUNCTION()
	virtual void OnDamageReceivedHandler(FDamageData DamageData);
	
};

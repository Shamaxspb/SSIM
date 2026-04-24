// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseDamageReactionComponent.h"

#include "SSIMEnemyDamageReactionComponent.generated.h"


class USSIMEnemyStatsComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyDamageReactionComponent : public USSIMBaseDamageReactionComponent
{
	GENERATED_BODY()
	
// Variables
	UPROPERTY()
	TObjectPtr<USSIMEnemyStatsComponent> EnemyStatsComponent;

// Overriden Functions
protected:
	virtual void BeginPlay() override;
	virtual void SetReferences() override;
	
// My Functions
protected:
	virtual void OnDamageReceivedHandler(FDamageData DamageData) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "SSIMProgressionSaveData.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class SSIM_API USSIMProgressionSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 CollectedHealthShards;
	
	UPROPERTY()
	int32 CollectedEnergyShards;
	
	UPROPERTY()
	FGameplayTagContainer UnlockedInnateAbilities;
	
};

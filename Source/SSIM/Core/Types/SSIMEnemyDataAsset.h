// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SSIMEnemyDataAsset.generated.h"


class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FEnemyAttributeDefaults
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes",	meta = (ClampMin = "1.0"))
	float MaxHealth = 30.f;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes",	meta = (ClampMin = "0.0"))
	float Health = 30.f;
};


UCLASS()
class SSIM_API USSIMEnemyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEnemyAttributeDefaults InitialAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RewardCredits = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InitializationEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> UnitAbilities;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "SSIMInnateAbilitiesCatalog.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FSSIMInnateAbilityDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ProgressionAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbility> AbilityClass;
};


UCLASS()
class SSIM_API USSIMInnateAbilitiesCatalog : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSSIMInnateAbilityDefinition> InnateAbilities;
};

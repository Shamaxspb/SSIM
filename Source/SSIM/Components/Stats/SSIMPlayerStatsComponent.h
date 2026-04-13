// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseStatsComponent.h"

#include "SSIMPlayerStatsComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMPlayerStatsComponent : public USSIMBaseStatsComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))	
	int32 MaxHealth = 5;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))	
	int32 Health = MaxHealth;

#pragma region Metadata
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stats|Damage")
	int32 ReceivedDamage;
	
#pragma endregion Metadata
	
// My Functions
public:
	void SetReceivedDamage(int32 InReceivedDamage);
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseStatsComponent.h"

#include "SSIMEnemyStatsComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyStatsComponent : public USSIMBaseStatsComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))
	float MaxHealth = 50.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stats|Health", meta = (ClampMin = 0))
	float Health = MaxHealth;
	
#pragma region Metadata
	
	UPROPERTY(BlueprintReadWrite, Category = "SSIM|Combat|Stats|Damage")
	float ReceivedDamage;
	
#pragma endregion Metadata
	
// My Functions
public:
	void SetReceivedDamage(float InReceivedDamage);
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	virtual void ReduceHealth() override;
	
};

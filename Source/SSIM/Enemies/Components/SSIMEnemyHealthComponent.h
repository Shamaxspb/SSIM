// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseEnemyComponent.h"
#include "SSIMEnemyHealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMEnemyHealthComponent : public USSIMBaseEnemyComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Combat|Stats")
	float MaxHealth = 50.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SSIM|Combat|Stats")
	float Health = MaxHealth;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Combat|Stats")
	void ReduceHealth(float InDamage);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMBaseDamageReactionComponent.generated.h"

class USSIMBaseStatsComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SSIM_API USSIMBaseDamageReactionComponent : public USSIMBaseComponent
{
	GENERATED_BODY()

// Variables
protected:
	FDamageData DamageData;

	UPROPERTY()
	TObjectPtr<USSIMBaseStatsComponent> BaseStatsComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Animations", meta = (DisplayPriority = 1))
	TObjectPtr<UAnimMontage> FrontStaggeredMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Animations")
	TObjectPtr<UAnimMontage> DeathMontage;

#pragma region Metadata
	
	FVector ReboundLaunchVelocity = FVector::ZeroVector;

#pragma endregion Metadata
	
#pragma region Rebound Debug
	
	// DEBUG
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound")
	bool bShowReboundLogs;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound")
	bool bDrawReboundDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bDrawReboundDebug", EditConditionHides))
	float ReboundDrawDuration = 3.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bDrawReboundDebug", EditConditionHides))
	float ReboundDirectionArrowLength = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bDrawReboundDebug", EditConditionHides))
	FLinearColor ReboundDirectionArrowColor = FLinearColor(1.f, 0.148f, 0.106f, 1.f);
	
#pragma endregion Rebound Debug
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Stagger")
	bool bShowStaggerLogs;
	
// Overriden Functions
public:
	virtual void BeginPlay() override;

// My Functions
protected:
	virtual void SetReferences() override;
	
	UFUNCTION()
	virtual void OnDamageReceivedHandler(const FDamageData& InDamageData);
	
	virtual void ReboundOnHit(UAnimMontage* InReboundMontage);
	
	virtual void ReboundOnDeath();
	
// Debug
protected:
	virtual void ReboundDrawDebug();
	
};

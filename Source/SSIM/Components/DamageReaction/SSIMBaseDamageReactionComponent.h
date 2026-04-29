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
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageProcessing|Animations", meta = (DisplayPriority = 1))
	TObjectPtr<UAnimMontage> FrontStaggeredMontage;

#pragma region Rebound Debug
	
	// DEBUG
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound")
	bool bReboundShowDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bReboundShowDebug", EditConditionHides))
	bool bReboundShowLogs;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bReboundShowDebug", EditConditionHides))
	bool bDrawReboundDirectionArrow;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bReboundShowDebug && bDrawReboundDirectionArrow", EditConditionHides))
	float DrawDuration = 3.f;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Rebound", meta = (EditCondition = "bReboundShowDebug && bDrawReboundDirectionArrow", EditConditionHides))
	FLinearColor ReboundDirectionArrowColor = FLinearColor(1.f, 0.148f, 0.106f, 1.f);
	
#pragma endregion Rebound Debug
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Stagger")
	bool bStaggerShowLogs;
	
// Overriden Functions
protected:
	virtual void BeginPlay() override;

// My Functions
protected:
	virtual void SetReferences() override;
	
	UFUNCTION()
	virtual void OnDamageReceivedHandler(const FDamageData& InDamageData);
	
	virtual void ReboundDrawDebug();
	
};

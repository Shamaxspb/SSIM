// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"

#include "SSIMPlayerFlowComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashingStateChangedSignature, bool, InDashingState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanDashChangedSignature, bool, InCanDashState);

class UCapsuleComponent;
class USSIMPlayerStatsComponent;
class ASSIMPlayer;

struct FDamageData;


UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerFlowComponent : public USSIMBaseComponent
{
	GENERATED_BODY()

// Variables
public:
	// Delegates
	FOnDashingStateChangedSignature OnDashingStateChangedDelegate;
	FOnCanDashChangedSignature OnCanDashChangedDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashCooldown = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashVelocityCoef = 15.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Dash")
	TObjectPtr<UAnimMontage> PlayerDashMontage;

#pragma region Metadata

public:
	FTimerHandle DashCooldownTimerHandle;
	
private:	
	bool bCanDash = true;

#pragma endregion Metadata
	
private:
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> StatsComponent;
	
	
// Overriden Functions
protected:
	virtual void BeginPlay() override;
	
	
// My Functions
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Dash")
	void StartDash();
	void EndDash();

private:
	FVector GetDashLaunchVelocity() const;
	
	void ResetDash();
	UFUNCTION()
	void ResetDashFromAir(const FHitResult& Hit);
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& DamageData);
	
};

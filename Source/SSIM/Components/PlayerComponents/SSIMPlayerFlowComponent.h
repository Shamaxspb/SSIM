// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"

#include "SSIMPlayerFlowComponent.generated.h"

class USSIMPlayerCombatComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashEndedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanDashChangedSignature, bool, InCanDash);


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
	FOnDashStartedSignature OnDashStartedDelegate;
	FOnDashEndedSignature OnDashEndedDelegate;
	
	FOnCanDashChangedSignature OnCanDashChangedDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashCooldown = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashVelocityCoef = 15.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashGravityScale = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashBrakingDecelerationWalking = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashBrakingDecelerationFalling = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Dash")
	TObjectPtr<UAnimMontage> PlayerDashMontage;

#pragma region Metadata

public:
	FTimerHandle DashCooldownTimerHandle;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Dash")
	bool bShowDashDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Dash", meta = (EditCondition = "bShowDashDebug", EditConditionHides))
	bool bShowDashLogs;

#pragma endregion Metadata
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> PlayerStatsComponent;
	UPROPERTY()
	TObjectPtr<USSIMPlayerCombatComponent> PlayerCombatComponent;
	
	
// Overriden Functions
protected:
	virtual void BeginPlay() override;
	
	
// My Functions
protected:
	virtual void SetReferences() override;
	
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
	void ResetBrakingDecelerationFalling(const FHitResult& Hit);
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& DamageData);
	
	UFUNCTION()
	void OnAttackStartedHandler();
	
};

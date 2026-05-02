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
	
	bool bDashing;
	bool bCanDash = true;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Dash")
	TObjectPtr<UAnimMontage> PlayerDashMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashDuration = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashVelocity = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashGravityScale = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashBrakingDecelerationWalking = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashBrakingDecelerationFalling = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashCooldown = 2.f;

#pragma region Metadata

public:
	FTimerHandle DashCooldownTimerHandle;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Dash")
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

	void ResetDash();
	UFUNCTION()
	void ResetDashFromAir();
	UFUNCTION()
	void ResetDashOnPogo();
	
	UFUNCTION()
	void OnLandedFromDashHandler(const FHitResult& Hit);
	
private:
	FVector GetDashLaunchVelocity() const;
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& DamageData);
	
};

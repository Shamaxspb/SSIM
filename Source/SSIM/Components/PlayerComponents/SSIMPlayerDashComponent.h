// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"

#include "SSIMPlayerDashComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashEndedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanDashChangedSignature, bool, InCanDash);


class UCapsuleComponent;
class USSIMPlayerStatsComponent;
class ASSIMPlayer;

struct FDamageData;


UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerDashComponent : public USSIMBaseComponent
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
	
#pragma region Settings
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animation")
	TObjectPtr<UAnimMontage> PlayerDashMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	float DashDuration = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	float DashVelocity = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	float DashGravityScale = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	float DashBrakingDecelerationWalking = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	float DashBrakingDecelerationFalling = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	float DashCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashHitRegistrationCollisionHalfHeight = 80.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashHitRegistrationCollisionRadius = 24.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashContactDamageCollisionHalfHeight = 105.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DamageCollision")
	float DashContactDamageCollisionRadius = 35.f;
	
#pragma endregion Settings

#pragma region Metadata

public:
	FTimerHandle DashCooldownTimerHandle;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG")
	bool bShowDashLogs;

#pragma endregion Metadata
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	FTimerHandle DashInProcessTimerHandle;
	
	
// Overriden Functions
public:
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
	
	void SetDashDamageCollision() const;
	UFUNCTION()
	void SetDefaultDamageCollision() const;
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& DamageData);
	
};

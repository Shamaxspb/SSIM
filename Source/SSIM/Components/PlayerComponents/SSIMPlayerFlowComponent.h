// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Components/SSIMBaseComponent.h"

#include "SSIMPlayerFlowComponent.generated.h"


class ASSIMPlayer;

UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerFlowComponent : public USSIMBaseComponent
{
	GENERATED_BODY()

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashCooldown = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashVelocityCoef = 15.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Dash")
	TObjectPtr<UAnimMontage> PlayerDashAnimation;

#pragma region Metadata

public:
	bool bDashing = false;
	
private:	
	bool bCanDash = true;
	FTimerHandle DashCooldownTimerHandle;

#pragma endregion Metadata
	
protected:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	
// Overriden Functions
public:
	virtual void BeginPlay() override;
	
	
// My Functions
	
public:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Dash")
	void StartDash();
	void EndDash();
	

protected:
	virtual void SetReferences() override;

private:
	FVector GetDashLaunchVelocity() const;
	void ResetDash();
	
	
	// DEBUG
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG|TakeDamage")
	void TakeDamageFromNearestEnemy();
	
};

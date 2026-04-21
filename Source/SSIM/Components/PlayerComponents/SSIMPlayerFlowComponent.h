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
	
#pragma region Rebound on Receive Damage
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|OnDamageRebound")
	float ReboundAngle = 45.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|OnDamageRebound")
	float ReboundVelocityCoefY = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|OnDamageRebound")
	float ReboundVelocityCoefZ = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|OnDamageRebound")
	float ReboundDelay = 1.f;
	
	// Debug
	UPROPERTY(EditAnywhere, Category = "SSIM|OnDamageRebound|DEBUG")
	bool bReboundShowDebug;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|OnDamageRebound|DEBUG", meta = (EditCondition = "bReboundShowDebug", EditConditionHides))
	bool bDrawReboundDirectionArrow;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|OnDamageRebound|DEBUG", meta = (EditCondition = "bReboundShowDebug && bDrawReboundDirectionArrow", EditConditionHides))
	FLinearColor ReboundDirectionArrowColor = FLinearColor(0.8f, 0.487f, 0.017f, 1.f);
	
#pragma endregion Rebound on Receive Damage
	
	
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Movement")
	/*const*/ float DEFAULT_GRAVITY_SCALE = 7.f;
	
	
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
	
	
	UFUNCTION()
	void OnDamageReceivedRebound(int32 NewHealth, AActor* InDamageInstigator);
	
	void ReceivedDamageLogicDelayed(AActor* InDamageInstigator);
	FVector CalculateReboundVelocity(AActor* InDamageInstigator) const;
	
	// DEBUG
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG|TakeDamage")
	void TakeDamageFromNearestEnemy();
	
};

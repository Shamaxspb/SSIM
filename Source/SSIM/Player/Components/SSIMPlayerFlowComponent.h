// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSIMPlayerFlowComponent.generated.h"


class ASSIMPlayer;

UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerFlowComponent : public UActorComponent
{
	GENERATED_BODY()

// Variables
public:
	bool bDashing;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Dash")
	float DashVelocityCoef = 15.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Dash")
	TObjectPtr<UAnimMontage> PlayerDashAnimation;
	
private:
	ASSIMPlayer* SSIMPlayer;
	
// Overriden Functions
public:
	USSIMPlayerFlowComponent();

protected:
	virtual void BeginPlay() override;
	
	virtual void InitializeComponent() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

// My Functions
public:
	FORCEINLINE void UseDash() const { Dash(); }
	
protected:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Dash")
	void Dash() const;
	
private:
	FVector GetDashLaunchVelocity() const;
	
};

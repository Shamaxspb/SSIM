// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "SSIMAIController.generated.h"

class UBehaviorTree;


UCLASS()
class SSIM_API ASSIMAIController : public AAIController
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|AI")
	TObjectPtr<UBehaviorTree> BaseBehaviorTree;

// Overriden Functions
public:
	ASSIMAIController();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	
};

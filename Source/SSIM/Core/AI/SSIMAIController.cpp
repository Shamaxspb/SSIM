// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAIController.h"

#include "SSIM/SSIM.h"


ASSIMAIController::ASSIMAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASSIMAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASSIMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!BaseBehaviorTree)
	{
		if (GetWorld()->IsGameWorld())
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Base Behavior Tree is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	RunBehaviorTree(BaseBehaviorTree);
}

void ASSIMAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


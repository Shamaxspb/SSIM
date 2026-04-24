// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Helpers/SSIMBlackboardHelper.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"


ASSIMAIController::ASSIMAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASSIMAIController::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	EnemyStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &ASSIMAIController::OnDamageReceivedHandler);
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


void ASSIMAIController::SetReferences()
{
	BaseEnemy = Cast<ASSIMBaseEnemy>(GetPawn());
	EnemyStatsComponent = BaseEnemy->GetEnemyStatsComponent();
}

void ASSIMAIController::OnDamageReceivedHandler(FDamageData InDamageData)
{
	SetBlackboardEnemyState(EEnemyState::EES_Staggered);
}

void ASSIMAIController::SetBlackboardEnemyState(EEnemyState InNewState)
{
	uint8 NewState = static_cast<uint8>(InNewState);
	USSIMBlackboardHelper::SetEnumSafe(Blackboard, TEXT("EEnemyState"), NewState);
	
	/*if (!IsValid(GetBlackboardComponent()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | BlackboardComponent is not valid"), TEXT(__FUNCTION__));
		return;
	}
	UBlackboardData* BBAsset = Blackboard->GetBlackboardAsset();
	if (!BBAsset || BBAsset->GetKeyID(TEXT("EEnemyState")) == FBlackboard::InvalidKey)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | There is no EEnemyState key in Blackboard"), TEXT(__FUNCTION__));
		return;
	}
	Blackboard->SetValueAsEnum(TEXT("EEnemyState"), static_cast<uint8>(InNewState));*/
	
}



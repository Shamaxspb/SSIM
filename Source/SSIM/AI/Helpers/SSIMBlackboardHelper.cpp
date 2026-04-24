// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBlackboardHelper.h"

#include "SSIM/SSIM.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"

class UBlackboardData;

void USSIMBlackboardHelper::SetEnumSafe(UBlackboardComponent* BB, FName KeyName, uint8 Value)
{
	if (!BB)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | BlackboardComponent is not valid"), TEXT(__FUNCTION__));
		return;
	}
	UBlackboardData* BBAsset = BB->GetBlackboardAsset();
	if (!BBAsset || BBAsset->GetKeyID(TEXT("EEnemyState")) == FBlackboard::InvalidKey)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | There is no EEnemyState key in Blackboard"), TEXT(__FUNCTION__));
		return;
	}
	BB->SetValueAsEnum(TEXT("EEnemyState"), static_cast<uint8>(Value));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBlackboardHelper.h"

#include "SSIM/SSIM.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"

class UBlackboardData;

void USSIMBlackboardHelper::SetEnumSafe(UBlackboardComponent* BB, FName KeyName, uint8 Value)
{
	if (IsBlackboardAndKeyValid(BB, KeyName))
	{
		BB->SetValueAsEnum(KeyName, Value);
	}
}

void USSIMBlackboardHelper::SetFloatSafe(UBlackboardComponent* BB, FName KeyName, float Value)
{
	if (IsBlackboardAndKeyValid(BB, KeyName))
	{
		BB->SetValueAsFloat(KeyName, Value);
	}
}

bool USSIMBlackboardHelper::IsBlackboardAndKeyValid(const UBlackboardComponent* BB, FName KeyName)
{
	if (!BB)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | BlackboardComponent is not valid"), TEXT(__FUNCTION__));
		return false;
	}
	UBlackboardData* BBAsset = BB->GetBlackboardAsset();
	if (!BBAsset || BBAsset->GetKeyID(KeyName) == FBlackboard::InvalidKey)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | There is no %s key in Blackboard"), TEXT(__FUNCTION__), *KeyName.ToString());
		return false;
	}
	return true;
}

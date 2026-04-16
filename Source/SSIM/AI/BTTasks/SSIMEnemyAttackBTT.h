// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBTTaskBase.h"
#include "SSIMEnemyAttackBTT.generated.h"

/**
 * 
 */
UCLASS()
class SSIM_API USSIMEnemyAttackBTT : public USSIMBTTaskBase
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp; 
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void OnEndAttack() const;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyAttackBTT.h"

#include "AIController.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Components/Combat/SSIMEnemyCombatComponent.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"

EBTNodeResult::Type USSIMEnemyAttackBTT::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	
	if (!IsValid(OwnerComp.GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner is not valid"), TEXT(__FUNCTION__));
		return EBTNodeResult::Failed;
	}
	BaseEnemy = Cast<ASSIMBaseEnemy>(CachedOwnerComp->GetAIOwner()->GetPawn());
	
	if (!BaseEnemy->Implements<USSIMCombatInterface>())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner does not implement USSIMCommonCombatInterface"), TEXT(__FUNCTION__));
		return EBTNodeResult::Failed;
	}
	
	BaseEnemy->GetEnemyCombatComponent()->OnAttackEndedDelegate.AddUniqueDynamic(this, &USSIMEnemyAttackBTT::OnAttackEndedHandler);
	
	ISSIMCombatInterface::Execute_StartAttackInterface(BaseEnemy);
	
	return EBTNodeResult::InProgress;
	
}

void USSIMEnemyAttackBTT::OnAttackEndedHandler()
{
	if (CachedOwnerComp.IsValid())
	{
		BaseEnemy->GetEnemyCombatComponent()->OnAttackEndedDelegate.RemoveDynamic(this, &USSIMEnemyAttackBTT::OnAttackEndedHandler);
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

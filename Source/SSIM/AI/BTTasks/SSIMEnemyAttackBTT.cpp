// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyAttackBTT.h"

#include "AIController.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"

EBTNodeResult::Type USSIMEnemyAttackBTT::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	
	if (!IsValid(OwnerComp.GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner is not valid"), TEXT(__FUNCTION__));
		return EBTNodeResult::Failed;
	}
	ASSIMBaseCharacter* BaseCharacter = Cast<ASSIMBaseCharacter>(CachedOwnerComp->GetAIOwner()->GetPawn());
	
	if (!BaseCharacter->Implements<USSIMCombatInterface>())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner does not implement USSIMCommonCombatInterface"), TEXT(__FUNCTION__));
		return EBTNodeResult::Failed;
	}
	
	BaseCharacter->OnAttackFinishedDelegate.AddUObject(this, &USSIMEnemyAttackBTT::OnEndAttack);
	
	ISSIMCombatInterface::Execute_StartAttackInterface(BaseCharacter);
	
	return EBTNodeResult::InProgress;
	
}

void USSIMEnemyAttackBTT::OnEndAttack() const
{
	if (CachedOwnerComp.IsValid())
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

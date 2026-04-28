// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAttackProcessingNotifyState.h"
#include "SSIM/SSIM.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"


void USSIMAttackProcessingNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!IsValid(MeshComp->GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	Owner = MeshComp->GetOwner();
									
	if (!Owner->Implements<USSIMCombatInterface>())  
	{
		if (IsValid(GetWorld()))
		{
			if (GetWorld()->IsGameWorld())
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s : %s does not implement USSIMCombatInterface"), TEXT(__FUNCTION__), *Owner->GetName());
			}
		}
		return;
	}
	
	ISSIMCombatInterface::Execute_StartAttackTraceInterface(Owner);
}

void USSIMAttackProcessingNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (!IsValid(Owner))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	if (!Owner->Implements<USSIMCombatInterface>())  
	{
		if (IsValid(GetWorld()))
		{
			if (GetWorld()->IsGameWorld())
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s : %s does not implement USSIMCombatInterface"), TEXT(__FUNCTION__), *Owner->GetName());
			}
		}
		return;
	}
	
	ISSIMCombatInterface::Execute_EndAttackTraceInterface(Owner);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAttackProcessingNotifyState.h"
#include "SSIM/SSIM.h"
#include "SSIM/Core/Interfaces/PlayerDataInterface.h"


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
	
									
	if (!Owner->Implements<UPlayerDataInterface>())  
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner does not implement UPlayerDataInterface"), TEXT(__FUNCTION__));
		return;
	}
	
	IPlayerDataInterface::Execute_StartAttackTraceInterface(Owner);
	
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
	
	if (!Owner->Implements<UPlayerDataInterface>())  
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Owner does not implement UPlayerDataInterface"), TEXT(__FUNCTION__));
		return;
	}
	
	IPlayerDataInterface::Execute_EndAttackTraceInterface(Owner);
}

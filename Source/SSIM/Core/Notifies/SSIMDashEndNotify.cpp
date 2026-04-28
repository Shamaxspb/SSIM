// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMDashEndNotify.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Interfaces/PlayerDataInterface.h"


void USSIMDashEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp->GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Owner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	if (!MeshComp->GetOwner()->Implements<UPlayerDataInterface>())  
	{
		if (IsValid(GetWorld()))
		{
			if (GetWorld()->IsGameWorld())
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s | Owner does not implement UPlayerDataInterface"), TEXT(__FUNCTION__));
			}
		}
		return;
	}
	
	IPlayerDataInterface::Execute_EndDashInterface(MeshComp->GetOwner());
}

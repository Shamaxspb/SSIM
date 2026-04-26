// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAttackEndNotify.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"


void USSIMAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp->GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Owner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	if (!MeshComp->GetOwner()->Implements<USSIMCombatInterface>())  
	{
		if (IsValid(GetWorld()))
		{
			if (GetWorld()->IsGameWorld())
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s | %s does not implement USSIMCombatInterface"), TEXT(__FUNCTION__), *MeshComp->GetOwner()->GetName());
			}
		}
		return;
	}
	
	ISSIMCombatInterface::Execute_EndAttackInterface(MeshComp->GetOwner());
}

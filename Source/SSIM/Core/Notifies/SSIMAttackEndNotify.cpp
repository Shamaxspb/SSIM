// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAttackEndNotify.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
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
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Owner does not implement USSIMCombatInterface"), TEXT(__FUNCTION__));
		return;
	}
	
	ASSIMBaseCharacter* BaseCharacter = Cast<ASSIMBaseCharacter>(MeshComp->GetOwner());
	BaseCharacter->OnAttackFinishedDelegate.Broadcast();
	
	ISSIMCombatInterface::Execute_EndAttackInterface(MeshComp->GetOwner());
}

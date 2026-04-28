// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMResetDashParametersNotify.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"

void USSIMResetDashParametersNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!IsValid(MeshComp->GetOwner()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Owner is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (IsValid(Character->GetCharacterMovement()))
	{
		Character->GetCharacterMovement()->GravityScale = GravityScale;
	}
}

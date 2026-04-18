// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SSIMAttackEndNotify.generated.h"


UCLASS(meta = (DisplayName = "SSIM Attack End"))
class SSIM_API USSIMAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
					    const FAnimNotifyEventReference& EventReference) override;	
};

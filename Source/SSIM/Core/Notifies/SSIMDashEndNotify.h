// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SSIMDashEndNotify.generated.h"


UCLASS(meta = (DisplayName = "SSIM Dash End"))
class SSIM_API USSIMDashEndNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
						const FAnimNotifyEventReference& EventReference) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SSIMResetDashParametersNotify.generated.h"


UCLASS(meta = (DisplayName = "SSIM Reset Dash Parameters"))
class SSIM_API USSIMResetDashParametersNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere ,BlueprintReadOnly)
	float GravityScale = 7.f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
						const FAnimNotifyEventReference& EventReference) override;
};

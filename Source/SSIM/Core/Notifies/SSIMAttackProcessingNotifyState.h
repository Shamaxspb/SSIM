// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SSIMAttackProcessingNotifyState.generated.h"

class AActor;

UCLASS(meta = (DisplayName = "Attack Processing"))
class SSIM_API USSIMAttackProcessingNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
// Variables
private:
	UPROPERTY()
	AActor* Owner;
	
// Overriden Functions
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd  (USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

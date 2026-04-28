// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/Animation/SSIMAnimInstance.h"

#include "SSIMPlayerAnimInstance.generated.h"

struct FDamageData;
class ASSIMPlayer;

struct FPogoBoneRotation
{
	FRotator* CurrentRotation;
	FRotator ModifiedRotation;
};

UCLASS()
class SSIM_API USSIMPlayerAnimInstance : public USSIMAnimInstance
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
#pragma region Pogo Modify Bones
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo|CurrentRotation")
	FRotator PogoCurrentAdditionalRotation_Pelvis   = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo|CurrentRotation")
	FRotator PogoCurrentAdditionalRotation_Spine_01 = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo|CurrentRotation")
	FRotator PogoCurrentAdditionalRotation_Spine_02 = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo|CurrentRotation")
	FRotator PogoCurrentAdditionalRotation_Thigh_L  = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo|CurrentRotation")
	FRotator PogoCurrentAdditionalRotation_Thigh_R  = FRotator::ZeroRotator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|ModifiedRotation")
	FRotator PogoModifiedAdditionalRotation_Pelvis = FRotator(0.f, 0.f, 50.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|ModifiedRotation")
	FRotator PogoModifiedAdditionalRotation_Spine_01 = FRotator(0.f, 0.f, 15.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|ModifiedRotation")
	FRotator PogoModifiedAdditionalRotation_Spine_02 = FRotator(0.f, 0.f, -15.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|ModifiedRotation")
	FRotator PogoModifiedAdditionalRotation_Thigh_L = FRotator(0.f, 0.f, 30.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|ModifiedRotation")
	FRotator PogoModifiedAdditionalRotation_Thigh_R = FRotator(0.f, 0.f, 30.f);
	
private:
	//TArray<FRotator*> PogoBonesRotations;
	//TMap<FRotator, FRotator> PogoBonesModifiedRotationsMap;
	
#pragma endregion Pogo Modify Bones

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|Interpolation")
	float PogoBlendInDuration = 0.15f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|Interpolation")
	float PogoBlendOutDuration = 0.15f;
	
private:
	bool bBlendInPogoAttackBones;
	bool bBlendOutPogoAttackBones;
	float BlendElapsedTime;
	

// Overriden Functions
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
// My Functions
protected:
	virtual void SetOwnerReference() override;
	
private:
	void ModifyBonesForPogo();
	void ResetBonesAfterPogo();
	
	void BlendInPogoBone(float InDeltaSeconds, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation);
	void BlendOutPogoBone(float InBlendElapsedTime, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation);
	
#pragma region Reset Pogo Handlers
	
	UFUNCTION()
	void OnPogoAnimationStartedHandler();
	UFUNCTION()
	void OnPogoEndedHandler();
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);
	UFUNCTION()
	void OnPogoAnimationEndedHandler();
	UFUNCTION()
	void OnPlayerLanded(const FHitResult& Hit);
	
#pragma endregion Reset Pogo Handlers
};

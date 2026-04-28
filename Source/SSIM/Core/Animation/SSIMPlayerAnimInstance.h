// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/Animation/SSIMAnimInstance.h"

#include "SSIMPlayerAnimInstance.generated.h"

struct FDamageData;
class ASSIMPlayer;


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
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalRotation_Pelvis   = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalRotation_Spine_01 = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalRotation_Spine_02 = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalRotation_Thigh_L  = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalRotation_Thigh_R  = FRotator::ZeroRotator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalDeltaRotation_Pelvis = FRotator(0.f, 0.f, 50.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalDeltaRotation_Spine_01 = FRotator(0.f, 0.f, 15.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalDeltaRotation_Spine_02 = FRotator(0.f, 0.f, -15.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalDeltaRotation_Thigh_L = FRotator(0.f, 0.f, 30.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo")
	FRotator PogoAdditionalDeltaRotation_Thigh_R = FRotator(0.f, 0.f, 30.f);
	
#pragma endregion Pogo Modify Bones

// My Functions
public:
	virtual void NativeBeginPlay() override;
	
// My Functions
protected:
	virtual void SetOwnerReference() override;
	
private:
	void ModifyBonesForPogo();
	void ResetBonesAfterPogo();
	
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
	
};

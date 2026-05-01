// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Core/Animation/SSIMAnimInstance.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerAnimInstance.generated.h"

struct FDamageData;
class ASSIMPlayer;

UENUM(BlueprintType)
enum class EPogoBonesState : uint8
{
	EPBS_DefaultRotation	UMETA(DisplayName = "Default Rotation"),
	EPBS_BlendingIn			UMETA(DisplayName = "Blending In"),
	EPBS_ModifiedRotation	UMETA(DisplayName = "Modified Rotation"),
	EPBS_BlendingOut		UMETA(DisplayName = "Blending Out"),
};


UCLASS()
class SSIM_API USSIMPlayerAnimInstance : public USSIMAnimInstance
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|References")
	TObjectPtr<UAnimMontage> CurrentAttackMontage;
		

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

#pragma endregion Pogo Modify Bones

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|Interpolation")
	float DefaultPogoBlendInDuration = 0.15f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|Interpolation")
	float DefaultPogoBlendOutDuration = 0.15f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|Interpolation")
	float OnLandedPogoBlendInDuration = 0.11f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Pogo|Interpolation")
	float OnDamageReceivedPogoBlendOutDuration = 0.11f;
	
	// Initially set to default, will change later, depending on context
	float PogoBlendInDuration = DefaultPogoBlendInDuration; 
	float PogoBlendOutDuration = DefaultPogoBlendOutDuration;

private:
	EPogoBonesState PogoBonesState = EPogoBonesState::EPBS_DefaultRotation;
	float BlendElapsedTime;
	
	// Debug
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SSIM|DEBUG|Pogo")
	bool bShowPogoBlendLogs = false;

// Overriden Functions
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	
// My Functions
protected:
	virtual void SetOwnerReference() override;
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG|Pogo")
	FORCEINLINE EPogoBonesState GetPogoBonesState() const
	{
		return PogoBonesState;
	}
	
private:
	void StartBlendInPogoBones(float InBlendInDuration);
	void StartBlendOutPogoBones(float InBlendOutDuration);
	
	void BlendInPogoBone(float InDeltaSeconds, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation);
	void BlendOutPogoBone(float InBlendElapsedTime, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation);
	
	UFUNCTION()
	void OnHitRegistrationHandle(EPlayerAttackDirectionType InPlayerAttackDirectionType);
	
#pragma region Reset Pogo Handlers
	
	UFUNCTION()
	void OnPogoAnimationStartedHandler();
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);
	UFUNCTION()
	void OnPogoAnimationEndedHandler(bool bInterrupted);
	UFUNCTION()
	void OnPlayerLanded(const FHitResult& Hit);
	UFUNCTION()
	void OnDashStartedHandler();
	
#pragma endregion Reset Pogo Handlers
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"

void USSIMPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationStartedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoEndedHandler);
	SSIMPlayer->GetPlayerCombatComponent()->OnPogoAnimationEndedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler);
	
	SSIMPlayer->GetPlayerStatsComponent()->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnDamageReceivedHandler);
	SSIMPlayer->LandedDelegate.AddDynamic(this, &USSIMPlayerAnimInstance::OnPlayerLanded);
	
	// PogoBonesRotations.Add(&PogoCurrentAdditionalRotation_Pelvis);
	// PogoBonesRotations.Add(&PogoCurrentAdditionalRotation_Spine_01);
	// PogoBonesRotations.Add(&PogoCurrentAdditionalRotation_Spine_02);
	// PogoBonesRotations.Add(&PogoCurrentAdditionalRotation_Thigh_L);
	// PogoBonesRotations.Add(&PogoCurrentAdditionalRotation_Thigh_R);
	
	// PogoBonesModifiedRotationsMap.Emplace(PogoCurrentAdditionalRotation_Pelvis,   PogoModifiedAdditionalRotation_Pelvis);
	// PogoBonesModifiedRotationsMap.Emplace(PogoCurrentAdditionalRotation_Spine_01, PogoModifiedAdditionalRotation_Spine_01);
	// PogoBonesModifiedRotationsMap.Emplace(PogoCurrentAdditionalRotation_Spine_02, PogoModifiedAdditionalRotation_Spine_02);
	// PogoBonesModifiedRotationsMap.Emplace(PogoCurrentAdditionalRotation_Thigh_L,  PogoModifiedAdditionalRotation_Thigh_L);
	// PogoBonesModifiedRotationsMap.Emplace(PogoCurrentAdditionalRotation_Thigh_R,  PogoModifiedAdditionalRotation_Thigh_R);
	
	// TArray<FPogoBoneRotation*> PogoBones;
	// FPogoBoneRotation* Pelvis = {PogoCurrentAdditionalRotation_Pelvis, PogoModifiedAdditionalRotation_Pelvis};
	// FPogoBoneRotation* Pelvis;
	// Pelvis->CurrentRotation = &PogoCurrentAdditionalRotation_Pelvis;
	// Pelvis->ModifiedRotation = PogoModifiedAdditionalRotation_Pelvis;
	// PogoBones.Add(Pelvis);
}

void USSIMPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (bBlendInPogoAttackBones)
	{
		BlendElapsedTime += DeltaSeconds;
		
		float Alpha			= FMath::Clamp(BlendElapsedTime / PogoBlendInDuration, 0.f, 1.f);
		float SmoothedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
		
		// for (FRotator& PogoBoneRotation : PogoBonesRotations)
		// {
		// 	BlendInPogoBones(SmoothedAlpha, PogoBoneRotation, PogoBonesModifiedRotationsMap[PogoBoneRotation]);
		// }
		
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Pelvis,   PogoModifiedAdditionalRotation_Pelvis);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_01, PogoModifiedAdditionalRotation_Spine_01);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_02, PogoModifiedAdditionalRotation_Spine_02);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_L,  PogoModifiedAdditionalRotation_Thigh_L);
		BlendInPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_R,  PogoModifiedAdditionalRotation_Thigh_R);
		
		if (SmoothedAlpha >= 1.f)
		{
			bBlendInPogoAttackBones = false;
			BlendElapsedTime = 0.f;
		}
	}
	
	if (bBlendOutPogoAttackBones)
	{
		BlendElapsedTime += DeltaSeconds;
		
		float Alpha			= FMath::Clamp(BlendElapsedTime / PogoBlendInDuration, 0.f, 1.f);
		float SmoothedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
		
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Pelvis,   PogoModifiedAdditionalRotation_Pelvis);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_01, PogoModifiedAdditionalRotation_Spine_01);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Spine_02, PogoModifiedAdditionalRotation_Spine_02);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_L,  PogoModifiedAdditionalRotation_Thigh_L);
		BlendOutPogoBone(SmoothedAlpha, PogoCurrentAdditionalRotation_Thigh_R,  PogoModifiedAdditionalRotation_Thigh_R);
		
		if (SmoothedAlpha >= 1.f)
		{
			bBlendOutPogoAttackBones = false;
			BlendElapsedTime = 0.f;
		}
	}
}

void USSIMPlayerAnimInstance::SetOwnerReference()
{
	if (!IsValid(TryGetPawnOwner()))
	{
		if (GetWorld()->IsGameWorld()) // to prevent log spamming in editor
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : TryGetPawnOwner is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(TryGetPawnOwner());
}

void USSIMPlayerAnimInstance::ModifyBonesForPogo()
{
	BlendElapsedTime = 0.f;
	bBlendInPogoAttackBones = true;
	
	// Set value instantly
	/*PogoCurrentAdditionalRotation_Pelvis   = PogoModifiedAdditionalRotation_Pelvis;
	PogoCurrentAdditionalRotation_Spine_01 = PogoModifiedAdditionalRotation_Spine_01;
	PogoCurrentAdditionalRotation_Spine_02 = PogoModifiedAdditionalRotation_Spine_02;
	PogoCurrentAdditionalRotation_Thigh_L  = PogoModifiedAdditionalRotation_Thigh_L;
	PogoCurrentAdditionalRotation_Thigh_R  = PogoModifiedAdditionalRotation_Thigh_R;*/
}

void USSIMPlayerAnimInstance::ResetBonesAfterPogo()
{
	BlendElapsedTime = 0.f;
	bBlendOutPogoAttackBones = true;
	
	// Set value instantly
	/*PogoCurrentAdditionalRotation_Pelvis   = FRotator::ZeroRotator;
	PogoCurrentAdditionalRotation_Spine_01 = FRotator::ZeroRotator;
	PogoCurrentAdditionalRotation_Spine_02 = FRotator::ZeroRotator;
	PogoCurrentAdditionalRotation_Thigh_L  = FRotator::ZeroRotator;
	PogoCurrentAdditionalRotation_Thigh_R  = FRotator::ZeroRotator;*/
}

void USSIMPlayerAnimInstance::BlendInPogoBone(float InSmoothedAlpha, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation)
{
	/*float Alpha			= FMath::Clamp(BlendElapsedTime / PogoBlendInDuration, 0.f, 1.f);
	float SmoothedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);*/

	InPogoBoneRotation = UKismetMathLibrary::RLerp(
							FRotator::ZeroRotator,
							InModifiedRotation,
				   			InSmoothedAlpha,
	   			true);
	
	/*if (SmoothedAlpha >= 1.f)
	{
		PogoBlendInDuration = false;
		BlendElapsedTime = 0.f;
	}*/
}

void USSIMPlayerAnimInstance::BlendOutPogoBone(float InSmoothedAlpha, FRotator& InPogoBoneRotation, const FRotator InModifiedRotation)
{
	/*BlendElapsedTime += InBlendElapsedTime;
	
	float Alpha			= FMath::Clamp(BlendElapsedTime / PogoBlendInDuration, 0.f, 1.f);
	float SmoothedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);*/
	
	InPogoBoneRotation = UKismetMathLibrary::RLerp(
							InModifiedRotation,
							FRotator::ZeroRotator,
							   InSmoothedAlpha,
				   true);
	
	/*if (SmoothedAlpha >= 1.f)
	{
		PogoBlendInDuration = false;
		BlendElapsedTime = 0.f;
	}*/
}

void USSIMPlayerAnimInstance::OnPogoAnimationStartedHandler()
{
	ModifyBonesForPogo();
}

void USSIMPlayerAnimInstance::OnPogoEndedHandler()
{
	ResetBonesAfterPogo();
}

void USSIMPlayerAnimInstance::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	ResetBonesAfterPogo();
}

void USSIMPlayerAnimInstance::OnPogoAnimationEndedHandler()
{
	ResetBonesAfterPogo();
}

void USSIMPlayerAnimInstance::OnPlayerLanded(const FHitResult& Hit)
{
	ResetBonesAfterPogo();
}

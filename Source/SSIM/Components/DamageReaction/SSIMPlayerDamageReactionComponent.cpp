// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"


// Overriden functions
void USSIMPlayerDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StaggeredFirstFrame->BlendIn = StaggeredFirstFrameBlendInTime;
}

void USSIMPlayerDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(SSIMOwnerCharacter);
}

// My Functions
void USSIMPlayerDamageReactionComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	Super::OnDamageReceivedHandler(InDamageData);
	
	InitStagger();
}

void USSIMPlayerDamageReactionComponent::ReboundOnHit()
{
	ReboundLaunchVelocity.Y = ReboundVelocityY * SSIMPlayer->GetPlayerFacingDirectionValue() * -1.f;
	ReboundLaunchVelocity.Z = ReboundVelocityZ;
	
	// if Enemy is to the Right to Player
	if (DamageData.Instigator->GetActorLocation().Y > SSIMPlayer->GetActorLocation().Y)
	{
		ReboundLaunchVelocity.Y = ReboundVelocityY * -1.f; 
		SSIMPlayer->SetPlayerFacingDirection(EFacingDirection::EPD_Right);
	}
	else
	{
		ReboundLaunchVelocity.Y = ReboundVelocityY;
		SSIMPlayer->SetPlayerFacingDirection(EFacingDirection::EPD_Left);
	}
	
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	SSIMPlayer->PlayAnimMontage(FrontStaggeredMontage, 1.f);
	
	Super::ReboundOnHit();
}

#pragma region Stagger processing

void USSIMPlayerDamageReactionComponent::InitStagger()
{
	SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
	SSIMPlayer->GetCharacterMovement()->GravityScale = 0.0f;
	
	if (!IsValid(StaggeredFirstFrame))
	{
		
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredFirstFrame montage is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMPlayer->PlayAnimMontage(StaggeredFirstFrame, 0.f);
		
	bStaggered = true;
	OnStaggerStartedDelegate.Broadcast();
	
	if (bShowStaggerLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger STARTED (%s)"), TEXT(__FUNCTION__), *SSIMPlayer->GetName());
	}
	
	StartStaggerSequence();
}

void USSIMPlayerDamageReactionComponent::StartStaggerSequence()
{
	CurrentStaggerSequenceStep = 0;
	
	Steps = {
		{ StaggeredFirstFrameBlendInTime, [this]() {StartStopFrame();}},
		{StopFrameDuration * 0.001f,		[this]() {StartStagger();}},
		{StaggerDuration,					[this]() {EndStagger();}}
	};
	
	ExecuteNextStaggerSequenceStep();
}

void USSIMPlayerDamageReactionComponent::ExecuteNextStaggerSequenceStep()
{
	if (!Steps.IsValidIndex(CurrentStaggerSequenceStep))
	{
		if (bShowStaggerLogs)
		{
			UE_LOG(LogSSIMValidations, Log, TEXT("%s | Stagger Sequence ended"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	const FStaggerSequenceStep& Step = Steps[CurrentStaggerSequenceStep++];
	
	GetWorld()->GetTimerManager().SetTimer(
		StaggerSequenceHandle,
		FTimerDelegate::CreateLambda([this, Step]()
		{
			Step.Function();
			ExecuteNextStaggerSequenceStep();
		}),
		Step.Delay,
		false
		);
}

void USSIMPlayerDamageReactionComponent::StartStopFrame() const
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
	
}

void USSIMPlayerDamageReactionComponent::EndStopFrame() const
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void USSIMPlayerDamageReactionComponent::StartStagger()
{
	EndStopFrame();
	
	if (!IsValid(FrontStaggeredMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredAnimation montage is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	ReboundOnHit();
}

void USSIMPlayerDamageReactionComponent::EndStagger()
{
	bStaggered = false;
	OnStaggerEndedDelegate.Broadcast();
	SSIMPlayer->StopAnimMontage(FrontStaggeredMontage);
	SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
	
	if (bShowStaggerLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger ENDED (%s)"), TEXT(__FUNCTION__), *SSIMPlayer->GetName());
	}
}

#pragma endregion Stagger processing

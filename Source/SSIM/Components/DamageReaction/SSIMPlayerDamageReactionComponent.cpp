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
	SetReferences();
	
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
	
	FRotator PlayerRotation = SSIMPlayer->GetActorRotation(); 
	PlayerRotation.Yaw = FVector(DamageData.Instigator->GetActorLocation() - SSIMPlayer->GetActorLocation()).Rotation().Yaw;
	SSIMPlayer->SetActorRotation(PlayerRotation);
	
	bool  bEnemyIsToTheRight = DamageData.Instigator->GetActorLocation().Y > SSIMPlayer->GetActorLocation().Y;
	ReboundLaunchVelocity = FVector(
							0.0f, 
							ReboundVelocityY * (bEnemyIsToTheRight ? -1.f : 1.f), 
							ReboundVelocityZ);
	
	SSIMPlayer->LaunchCharacter(ReboundLaunchVelocity, true, true);
	
#if !UE_BUILD_SHIPPING
	if (bDrawReboundDebug)
	{
		ReboundDrawDebug();
	}
#endif !UE_BUILD_SHIPPING
	
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
	if (!IsValid(FrontStaggeredMontage))
    {
    	UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredAnimation montage is not valid"), TEXT(__FUNCTION__));
    	return;
    }
    SSIMPlayer->PlayAnimMontage(FrontStaggeredMontage, 1.f);
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

void USSIMPlayerDamageReactionComponent::ReboundDrawDebug()
{
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
								SSIMPlayer->GetActorLocation(), 
								 SSIMPlayer->GetActorLocation() + ReboundLaunchVelocity.GetSafeNormal() * 400.f, 
							   25.f, 
										 ReboundDirectionArrowColor, 
										 DrawDuration, 
							   5.f);
}

#pragma endregion Stagger processing

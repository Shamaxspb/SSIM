// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SSIM/SSIM.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden functions
USSIMPlayerDamageReactionComponent::USSIMPlayerDamageReactionComponent()
{
}

void USSIMPlayerDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();
	StaggeredFirstFrame->BlendIn = StaggeredFirstFrameBlendInTime;
	
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerDamageReactionComponent::InitStagger);
}

// My Functions
void USSIMPlayerDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	PlayerStatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMPlayerStatsComponent>();
}

#pragma region Stagger processing
void USSIMPlayerDamageReactionComponent::InitStagger(const FDamageData DamageData)
{
	SSIMOwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	SSIMOwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
	
	if (!IsValid(StaggeredFirstFrame))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredFirstFrame montage is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMOwnerCharacter->PlayAnimMontage(StaggeredFirstFrame, 0.f);
		
	PlayerStatsComponent->bStaggered = true;
	PlayerStatsComponent->bInvulnerable = true;
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger STARTED"), TEXT(__FUNCTION__));
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Invulnerability STARTED"), TEXT(__FUNCTION__));
	
	StartStaggerSequence();
}

void USSIMPlayerDamageReactionComponent::StartStaggerSequence()
{
	CurrentStaggerSequenceStep = 0;
	
	Steps = {
		{ StaggeredFirstFrameBlendInTime, [this]() {StartStopFrame();}},
		{StopFrameDuration * 0.001f,		[this]() {StopFrameToStagger();}},
		{StaggerDuration,					[this]() {EndStagger();}},
		{InvulnerabilityDuration,			[this]() {EndInvulnerability(); }}
	};
	
	ExecuteNextStaggerSequenceStep();
}

void USSIMPlayerDamageReactionComponent::ExecuteNextStaggerSequenceStep()
{
	if (!Steps.IsValidIndex(CurrentStaggerSequenceStep))
	{
		UE_LOG(LogSSIMValidations, Warning, TEXT("%s | Stagger Steps Array has ended (invalid index)"), TEXT(__FUNCTION__));
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

void USSIMPlayerDamageReactionComponent::StopFrameToStagger() const
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

	FVector ReboundLaunchVelocity = FVector(
							0.0f, 
							SSIMOwnerCharacter->GetActorForwardVector().Y * ReboundVelocityY * -1.f, 
								ReboundVelocityZ);
	SSIMOwnerCharacter->LaunchCharacter(ReboundLaunchVelocity, true, true);
	
	#if !UE_BUILD_SHIPPING
	if (bReboundShowDebug)
	{
		//FVector2D(ReboundVelocityY * -1.f, ReboundVelocityZ).Normalize() * 400.f;
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
									SSIMOwnerCharacter->GetActorLocation(), 
									 SSIMOwnerCharacter->GetActorLocation() + ReboundLaunchVelocity.Normalize() * 400.f, 
								   25.f, 
											 ReboundDirectionArrowColor, 
									 3.f, 
								   5.f);
	}
	#endif !UE_BUILD_SHIPPING
	
	SSIMOwnerCharacter->GetCharacterMovement()->GravityScale = DEFAULT_GRAVITY_SCALE;
	
	if (!IsValid(StaggeredAnimation))
    {
    	UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredAnimation montage is not valid"), TEXT(__FUNCTION__));
    	return;
    }
    SSIMOwnerCharacter->PlayAnimMontage(StaggeredAnimation, 1.f);
}

void USSIMPlayerDamageReactionComponent::EndStagger() const
{
	PlayerStatsComponent->bStaggered = false;
	SSIMOwnerCharacter->StopAnimMontage(StaggeredAnimation);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger ENDED"), TEXT(__FUNCTION__));
}

void USSIMPlayerDamageReactionComponent::EndInvulnerability() const
{
	PlayerStatsComponent->bInvulnerable = false;
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Invulnerability ENDED"), TEXT(__FUNCTION__));
}

#pragma endregion Stagger processing

// DEBUG 
void USSIMPlayerDamageReactionComponent::ManualStagger_DEBUG()
{
	InitStagger(FDamageData(SSIMOwnerCharacter, 1));
}

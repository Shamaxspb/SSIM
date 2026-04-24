// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SSIM/SSIM.h"
#include "SSIM/Components/Stats/SSIMBaseStatsComponent.h"


// Overriden functions
void USSIMPlayerDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	StaggeredFirstFrame->BlendIn = StaggeredFirstFrameBlendInTime;
	
}

// My Functions


void USSIMPlayerDamageReactionComponent::OnDamageReceivedHandler(FDamageData InDamageData)
{
	Super::OnDamageReceivedHandler(DamageData);
	
	InitStagger(DamageData);
}

#pragma region Stagger processing

void USSIMPlayerDamageReactionComponent::InitStagger(const FDamageData InDamageData)
{
	DamageData.Instigator = InDamageData.Instigator;
	DamageData.Value = InDamageData.Value;
	
	SSIMOwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	SSIMOwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
	
	if (!IsValid(StaggeredFirstFrame))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredFirstFrame montage is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMOwnerCharacter->PlayAnimMontage(StaggeredFirstFrame, 0.f);
		
	BaseStatsComponent->bStaggered = true;
	BaseStatsComponent->bInvulnerable = true;
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger STARTED"), TEXT(__FUNCTION__));
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Invulnerability STARTED"), TEXT(__FUNCTION__));
	
	StartStaggerSequence();
}

void USSIMPlayerDamageReactionComponent::StartStaggerSequence()
{
	CurrentStaggerSequenceStep = 0;
	
	Steps = {
		{ StaggeredFirstFrameBlendInTime, [this]() {StartStopFrame();}},
		{StopFrameDuration * 0.001f,		[this]() {StartStagger();}},
		{StaggerDuration,					[this]() {EndStagger();}},
		{InvulnerabilityDuration,			[this]() {EndInvulnerability(); }}
	};
	
	ExecuteNextStaggerSequenceStep();
}

void USSIMPlayerDamageReactionComponent::ExecuteNextStaggerSequenceStep()
{
	if (!Steps.IsValidIndex(CurrentStaggerSequenceStep))
	{
		UE_LOG(LogSSIMValidations, Log, TEXT("%s | Stagger Sequence ended"), TEXT(__FUNCTION__));
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

void USSIMPlayerDamageReactionComponent::StartStagger() const
{
	EndStopFrame();
	
	FRotator PlayerRotation = SSIMOwnerCharacter->GetActorRotation(); 
	PlayerRotation.Yaw = FVector(DamageData.Instigator->GetActorLocation() - SSIMOwnerCharacter->GetActorLocation()).Rotation().Yaw;
	SSIMOwnerCharacter->SetActorRotation(PlayerRotation);
	
	bool  bEnemyIsToTheRight = DamageData.Instigator->GetActorLocation().Y > SSIMOwnerCharacter->GetActorLocation().Y;
	FVector ReboundLaunchVelocity = FVector(
							0.0f, 
							ReboundVelocityY * (bEnemyIsToTheRight ? -1.f : 1.f), 
							ReboundVelocityZ);
	
	SSIMOwnerCharacter->LaunchCharacter(ReboundLaunchVelocity, true, true);
	
	#if !UE_BUILD_SHIPPING
	if (bReboundShowDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
									SSIMOwnerCharacter->GetActorLocation(), 
									 SSIMOwnerCharacter->GetActorLocation() + ReboundLaunchVelocity.GetSafeNormal() * 400.f, 
								   25.f, 
											 ReboundDirectionArrowColor, 
											 DrawDuration, 
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
	BaseStatsComponent->bStaggered = false;
	SSIMOwnerCharacter->StopAnimMontage(StaggeredAnimation);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger ENDED"), TEXT(__FUNCTION__));
}

void USSIMPlayerDamageReactionComponent::EndInvulnerability() const
{
	BaseStatsComponent->bInvulnerable = false;
	OnEndInvulnerabilityDelegate.Broadcast();
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Invulnerability ENDED"), TEXT(__FUNCTION__));
}

#pragma endregion Stagger processing

// DEBUG 
void USSIMPlayerDamageReactionComponent::ManualStagger_DEBUG()
{
	InitStagger(FDamageData(SSIMOwnerCharacter, 1));
}

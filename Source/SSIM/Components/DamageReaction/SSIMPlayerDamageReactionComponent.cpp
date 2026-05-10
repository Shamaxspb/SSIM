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
	
	if (!IsValid(StaggeredFirstFrame))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | StaggeredFirstFrame is not valid"), TEXT(__FUNCTION__));
		return;
	}
	StaggeredFirstFrame->BlendIn = StaggeredFirstFrameBlendInTime;
}

void USSIMPlayerDamageReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	InterpolateGravityToZeroOnDeath(DeltaTime);
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
	
	int32 PlayerHealth = SSIMPlayer->GetPlayerStatsComponent()->Health;
	if (PlayerHealth <= 0)
	{
		LethalDamageReaction();
		return;
	}
	
	InitStagger();
}

void USSIMPlayerDamageReactionComponent::ReboundOnHit(UAnimMontage* InReboundMontage)
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
	
	Super::ReboundOnHit(InReboundMontage);
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
	
	ReboundOnHit(FrontStaggeredMontage);
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

void USSIMPlayerDamageReactionComponent::LethalDamageReaction()
{
	ReboundLaunchVelocity = FVector(0.f, DeathReboundVelocityY, DeathReboundVelocityZ);
	
	// if Enemy is to the Right to Player
	if (DamageData.Instigator->GetActorLocation().Y > SSIMPlayer->GetActorLocation().Y)
	{
		ReboundLaunchVelocity.Y = DeathReboundVelocityY * -1.f; 
		SSIMPlayer->SetPlayerFacingDirection(EFacingDirection::EPD_Right);
	}
	else
	{
		ReboundLaunchVelocity.Y = DeathReboundVelocityY;
		SSIMPlayer->SetPlayerFacingDirection(EFacingDirection::EPD_Left);
	}
	
	ReboundOnDeath();
	
	GravityInterpolationElapsedTime = 0.f;
	bShouldInterpolateGravity = true;
	
	/*SSIMPlayer->GetCharacterMovement()->GravityScale = DeathReboundInitialGravity;
	
	FTimerHandle DeathGravityTimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(
		DeathGravityTimerHandle,
		this, &USSIMPlayerDamageReactionComponent::InterpolateGravityToZero,
		DeathGravityInterpolationDuration,
		false
		);*/
}

void USSIMPlayerDamageReactionComponent::InterpolateGravityToZeroOnDeath(float DeltaTime)
{
	if (bShouldInterpolateGravity)
	{
		GravityInterpolationElapsedTime += DeltaTime;
		
		const float Alpha = FMath::Clamp(GravityInterpolationElapsedTime / DeathGravityInterpolationDuration, 0.f, 1.f);
		
		SSIMPlayer->GetCharacterMovement()->GravityScale = 
			FMath::InterpEaseOut(DeathReboundInitialGravity, 0.f, Alpha, 2.f);
		
		if (Alpha >= 1.f)
		{
			EndReboundOnDeath();
		}
	}
}

void USSIMPlayerDamageReactionComponent::EndReboundOnDeath()
{
	SSIMPlayer->GetCharacterMovement()->GravityScale = 0.f;
	bShouldInterpolateGravity = false;
	GravityInterpolationElapsedTime = 0.f;
	
	AnimInstance->Montage_SetPlayRate(DeathMontage, 0.f);
}


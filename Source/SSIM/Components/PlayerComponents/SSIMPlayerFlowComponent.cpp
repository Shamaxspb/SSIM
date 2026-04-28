// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerFlowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden Functions
void USSIMPlayerFlowComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMPlayerStatsComponent>();
	
	if (!IsValid(StatsComponent))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | Stats component is not valid"), TEXT(__FUNCTION__));
	}
	StatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerFlowComponent::OnDamageReceivedHandler);
	
}


// My Functions
void USSIMPlayerFlowComponent::StartDash()
{	

	if (!IsValid(PlayerDashMontage))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		return;
	}
	AnimInstance->Montage_Play(PlayerDashMontage);
	
	
	
	SSIMOwnerCharacter->LaunchCharacter(GetDashLaunchVelocity() ,true, false);
	
	// Should implement OnCompleted/OnBlendOut/AnimNotify bDashing reset
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DashInProcessTimerHandle, this, &USSIMPlayerFlowComponent::EndDash, PlayerDashMontage->GetPlayLength(), false);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | PlayerDash animation length: %f"), TEXT(__FUNCTION__), PlayerDashMontage->GetPlayLength());
	
	if (SSIMOwnerCharacter->GetCharacterMovement()->IsFalling())
	{
		ASSIMPlayer* SSIMPlayer = Cast<ASSIMPlayer>(SSIMOwnerCharacter);
		SSIMPlayer->LandedDelegate.AddDynamic(this, &USSIMPlayerFlowComponent::ResetDashFromAir);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			DashCooldownTimerHandle, 
			this, 
			&USSIMPlayerFlowComponent::ResetDash, 
			DashCooldown, 
			false);
	}
	
	OnDashStartedDelegate.Broadcast();
	OnCanDashChangedDelegate.Broadcast(false);
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash started"), TEXT(__FUNCTION__));
}

FVector USSIMPlayerFlowComponent::GetDashLaunchVelocity() const
{
	// Calculate Player direction
	FVector DashDirectionVector;
	float DirectionDotProduct = FVector::DotProduct(SSIMOwnerCharacter->GetActorForwardVector(), FVector::RightVector);
		
	if (FMath::IsNearlyEqual(DirectionDotProduct, 1.f))
	{
		DashDirectionVector = FVector::RightVector;
	}
	else if (FMath::IsNearlyEqual(DirectionDotProduct, -1.f))
	{
		DashDirectionVector = FVector::RightVector * -1.f;
	}
	else
	{
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s | Couldn't determine player direction. Return -1.f"), TEXT(__FUNCTION__));
		return FVector(-1.f, -1.f, -1.f);
	}
	
	FVector OutLaunchVelocity =  DashDirectionVector *
								 SSIMOwnerCharacter->GetCharacterMovement()->GetMaxSpeed() *
								 DashVelocityCoef;
		
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Launch Velocity: %s"), TEXT(__FUNCTION__), *OutLaunchVelocity.ToString());
	return OutLaunchVelocity;
}

void USSIMPlayerFlowComponent::EndDash()
{
	OnDashEndedDelegate.Broadcast();
}

void USSIMPlayerFlowComponent::ResetDash()
{
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerFlowComponent::ResetDashFromAir(const FHitResult& Hit)
{
	ASSIMPlayer* SSIMPlayer = Cast<ASSIMPlayer>(SSIMOwnerCharacter);
	SSIMPlayer->LandedDelegate.RemoveDynamic(this, &USSIMPlayerFlowComponent::ResetDashFromAir);
	
	OnCanDashChangedDelegate.Broadcast(true);
}

void USSIMPlayerFlowComponent::OnDamageReceivedHandler(const FDamageData& DamageData)
{
	EndDash();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerFlowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "TimerManager.h"
#include "SSIM/Player/SSIMPlayer.h"
#include "SSIM/Core/Notifies/SSIMDashEndNotify.h"


// Overriden Functions
USSIMPlayerFlowComponent::USSIMPlayerFlowComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMPlayerFlowComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitDashAnimation();
}


void USSIMPlayerFlowComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


// My Functions
void USSIMPlayerFlowComponent::StartDash()
{
	if (bDashing || !bCanDash)
	{
		if (bDashing)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Dash is still in process"), TEXT(__FUNCTION__));
		}
		
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Dash is on cooldown for %f"), TEXT(__FUNCTION__), GetWorld()->GetTimerManager().GetTimerRemaining(DashCooldownTimerHandle));
		return;
	}
	
	bDashing = true;
	bCanDash = false;
	
	SSIMPlayer->LaunchCharacter(GetDashLaunchVelocity() ,true, false);

	if (!IsValid(PlayerDashAnimation))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		return;
	}
	SSIMAnimInstance->Montage_Play(PlayerDashAnimation);
	
	// Should implement OnCompleted/OnBlendOut/AnimNotify bDashing reset
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DashInProcessTimerHandle, this, &USSIMPlayerFlowComponent::EndDash, PlayerDashAnimation->GetPlayLength(), false);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : PlayerDash animation length: %f"), TEXT(__FUNCTION__), PlayerDashAnimation->GetPlayLength());
	
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &USSIMPlayerFlowComponent::ResetDash, DashCooldown, false);
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Dash started"), TEXT(__FUNCTION__));
}


FVector USSIMPlayerFlowComponent::GetDashLaunchVelocity() const
{
	FVector CurrentVelocity = SSIMPlayer->GetVelocity();
	FVector OutLaunchVelocity;
	
	if (CurrentVelocity.IsNearlyZero())
	{
		// Dash in place
		
		// Calculate Player direction
		FVector DashDirectionVector;
		float DirectionDotProduct = FVector::DotProduct(SSIMPlayer->GetActorForwardVector(), FVector::RightVector);
		
		
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
			UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s : Couldn't determine player direction. Return -1.f"), TEXT(__FUNCTION__));
			return FVector(-1.f, -1.f, -1.f);
		}
		
		OutLaunchVelocity =  DashDirectionVector *
							 SSIMPlayer->GetCharacterMovement()->GetMaxSpeed() *
							 DashVelocityCoef;
		
	}
	else
	{
		// Dash in motion
		OutLaunchVelocity = FVector(0.f, SSIMPlayer->GetVelocity().Y * DashVelocityCoef,0.f);
	}
		
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Dash Launch Velocity: %s"), TEXT(__FUNCTION__), *OutLaunchVelocity.ToString());
	return OutLaunchVelocity;
}


void USSIMPlayerFlowComponent::EndDash()
{
	bDashing = false;
}


void USSIMPlayerFlowComponent::ResetDash()
{
	bCanDash = true;
}

void USSIMPlayerFlowComponent::InitDashAnimation()
{
	if (PlayerDashAnimation)
	{
		const TArray<FAnimNotifyEvent> NotifyEvents = PlayerDashAnimation->Notifies;
		for (const FAnimNotifyEvent& EventNotify : NotifyEvents)
		{
			if (const auto DashEndNotify = Cast<USSIMDashEndNotify>(EventNotify.Notify))
			{
				DashEndNotify->OnNotified.AddUObject(this, &USSIMPlayerFlowComponent::DashEndNotify);
			}
		}
	}
}

void USSIMPlayerFlowComponent::DashEndNotify()
{
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Dash End notify reached"), TEXT(__FUNCTION__));	
}


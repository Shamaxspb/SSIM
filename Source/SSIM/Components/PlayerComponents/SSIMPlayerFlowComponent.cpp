// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerFlowComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden Functions
void USSIMPlayerFlowComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetReferences();
	
	SSIMPlayer->GetCharacterMovement()->GravityScale = DEFAULT_GRAVITY_SCALE;
}

void USSIMPlayerFlowComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(GetOwner());
}

// My Functions
void USSIMPlayerFlowComponent::StartDash()
{
	if (bDashing || !bCanDash)
	{
		if (bDashing)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is still in process"), TEXT(__FUNCTION__));
		}
		
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is on cooldown for %f"), TEXT(__FUNCTION__), GetWorld()->GetTimerManager().GetTimerRemaining(DashCooldownTimerHandle));
		return;
	}
	
	bDashing = true;
	bCanDash = false;
	
	SSIMOwnerCharacter->LaunchCharacter(GetDashLaunchVelocity() ,true, false);

	if (!IsValid(PlayerDashAnimation))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | PlayerDashAnimation is not valid"), TEXT(__FUNCTION__));
		return;
	}
	AnimInstance->Montage_Play(PlayerDashAnimation);
	
	// Should implement OnCompleted/OnBlendOut/AnimNotify bDashing reset
	FTimerHandle DashInProcessTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DashInProcessTimerHandle, this, &USSIMPlayerFlowComponent::EndDash, PlayerDashAnimation->GetPlayLength(), false);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | PlayerDash animation length: %f"), TEXT(__FUNCTION__), PlayerDashAnimation->GetPlayLength());
	
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &USSIMPlayerFlowComponent::ResetDash, DashCooldown, false);
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash started"), TEXT(__FUNCTION__));
}

FVector USSIMPlayerFlowComponent::GetDashLaunchVelocity() const
{
	FVector CurrentVelocity = SSIMOwnerCharacter->GetVelocity();
	FVector OutLaunchVelocity;
	
	if (CurrentVelocity.IsNearlyZero())
	{
		// Dash in place
		
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
		
		OutLaunchVelocity =  DashDirectionVector *
							 SSIMOwnerCharacter->GetCharacterMovement()->GetMaxSpeed() *
							 DashVelocityCoef;
		
	}
	else
	{
		// Dash in motion
		OutLaunchVelocity = FVector(0.f, SSIMOwnerCharacter->GetVelocity().Y * DashVelocityCoef,0.f);
	}
		
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash Launch Velocity: %s"), TEXT(__FUNCTION__), *OutLaunchVelocity.ToString());
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

void USSIMPlayerFlowComponent::OnDamageReceivedRebound(int32 NewHealth, AActor* InDamageInstigator)
{
	/*SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
	SSIMPlayer->GetCharacterMovement()->GravityScale = 0.f;
	
	FTimerHandle ReceivedDamageLogicDelayedTimerHandle;
	FTimerDelegate ReceivedDamageLogicDelayedTimerDelegate;
	ReceivedDamageLogicDelayedTimerDelegate.BindUObject(this, &USSIMPlayerFlowComponent::ReceivedDamageLogicDelayed, InDamageInstigator);
	GetWorld()->GetTimerManager().SetTimer(ReceivedDamageLogicDelayedTimerHandle, ReceivedDamageLogicDelayedTimerDelegate, ReboundDelay, false);*/
	
}

void USSIMPlayerFlowComponent::ReceivedDamageLogicDelayed(AActor* InDamageInstigator)
{
	/*FVector ReboundVelocity = CalculateReboundVelocity(InDamageInstigator);
	SSIMPlayer->LaunchCharacter(ReboundVelocity, true, true);*/
}

FVector USSIMPlayerFlowComponent::CalculateReboundVelocity(AActor* InDamageInstigator) const
{
	/*FVector PlayerLocation = SSIMPlayer->GetActorLocation();
	FVector EnemyLocation = InDamageInstigator->GetActorLocation();
	
	// Determine if Enemy is to the right or to the left
	bool bEnemyToTheRight = EnemyLocation.Y > PlayerLocation.Y;
	
	// Get unit vector from Enemy to Player and Negate that vector (around X axis)
	FVector ReboundDirection = UKismetMathLibrary::NegateVector(UKismetMathLibrary::GetDirectionUnitVector(EnemyLocation, PlayerLocation));
	
	// Add rotation to that vector
	FVector RotatedDirection = ReboundDirection.RotateAngleAxis(bEnemyToTheRight ? ReboundAngle : -ReboundAngle, FVector::ForwardVector);
	
	FVector ReboundVelocity = FVector(RotatedDirection.X, RotatedDirection.Y * ReboundVelocityCoefY, RotatedDirection.Z * ReboundVelocityCoefZ);

#if !UE_BUILD_SHIPPING
	if (bReboundShowDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), EnemyLocation, EnemyLocation + (RotatedDirection * 250.f), 25.f, ReboundDirectionArrowColor, 3.f, 5.f);
	}
#endif !UE_BUILD_SHIPPING
	
	return ReboundVelocity;*/
	return FVector::ZeroVector;
}


// DEBUG
void USSIMPlayerFlowComponent::TakeDamageFromNearestEnemy()
{
	
}

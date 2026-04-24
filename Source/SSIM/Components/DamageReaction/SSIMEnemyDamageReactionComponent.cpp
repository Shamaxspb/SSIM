// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyDamageReactionComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

//


void USSIMEnemyDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
}

void USSIMEnemyDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMEnemy = CastChecked<ASSIMBaseEnemy>(SSIMOwnerCharacter);
	EnemyStatsComponent = CastChecked<USSIMEnemyStatsComponent>(BaseStatsComponent);
}

void USSIMEnemyDamageReactionComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	Super::OnDamageReceivedHandler(InDamageData);
	
	StartStagger();
	ReboundOnHit();
	
}

void USSIMEnemyDamageReactionComponent::StartStagger()
{
	EnemyStatsComponent->EnemyState = EEnemyState::EES_Staggered;
	
	GetWorld()->GetTimerManager().SetTimer(
		StaggerTimerHandle,
		this, 
		&USSIMEnemyDamageReactionComponent::EndStagger,
		StaggerDuration,
		false
		);
}

void USSIMEnemyDamageReactionComponent::EndStagger()
{
	EnemyStatsComponent->EnemyState = EEnemyState::EES_Combat; // Since enemy can be staggered only in combat (not sure about this)
	EndStaggerDelegate.Broadcast();
}

void USSIMEnemyDamageReactionComponent::ReboundOnHit()
{
	FVector EnemyLocation = SSIMEnemy->GetActorLocation();
	FVector PlayerLocation = DamageData.Instigator->GetActorLocation();
	
	// Determine if Player is to the right or to the left
	bool bPlayerToTheRight = PlayerLocation.Y > EnemyLocation.Y;
	
	// Get unit vector from Player to Enemy and Negate that vector
	FVector ReboundDirection = UKismetMathLibrary::NegateVector(UKismetMathLibrary::GetDirectionUnitVector(EnemyLocation, PlayerLocation));
	
	// Add rotation to that vector (around X axis)
	RotatedDirection = ReboundDirection.RotateAngleAxis(bPlayerToTheRight ? -ReboundAngle : ReboundAngle, FVector::ForwardVector);

	// Multiply by coef for launch
	FVector ReboundVelocity = RotatedDirection * ReboundVelocityCoef;
	
#if !UE_BUILD_SHIPPING
	
	if (bReboundShowLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Player Is to the: %s"), (bPlayerToTheRight ? TEXT("Right") : TEXT("Left")));
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Rebound Direction: %s"), *ReboundDirection.ToString());
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Rotated Direction: %s"), *RotatedDirection.ToString());
		UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Rebound Velocity: %s"),  *ReboundVelocity.ToString());
	}
	
	if (bDrawReboundDirectionArrow)
	{
		ReboundDrawDebug();
	}

#endif !UE_BUILD_SHIPPING
	
	SSIMEnemy->LaunchCharacter(ReboundVelocity, false, false);
}

void USSIMEnemyDamageReactionComponent::ReboundDrawDebug()
{
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
								SSIMEnemy->GetActorLocation(), 
								 SSIMEnemy->GetActorLocation() + (RotatedDirection * 250.f), 
							   25.f, 
										 ReboundDirectionArrowColor, 
										 DrawDuration, 
							   5.f);
}

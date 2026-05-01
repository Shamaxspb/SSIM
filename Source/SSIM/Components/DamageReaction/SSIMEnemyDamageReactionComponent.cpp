// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyDamageReactionComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"
#include "SSIM/Core/Helpers/SSIMBlackboardHelper.h"
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
	
	if (!IsValid(SelectStaggerMontage()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | %s montage is not valid"), 
											    TEXT(__FUNCTION__), 
													 *GetNameSafe(SelectStaggerMontage()));
		return;
	}
	SSIMEnemy->PlayAnimMontage(SelectStaggerMontage(), 1.f);
	
	if (bShowStaggerLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger STARTED (%s)"), TEXT(__FUNCTION__), *SSIMEnemy->GetName());
	}
	
	ReboundOnHit();
	OnStartStaggerDelegate.Broadcast();
}

void USSIMEnemyDamageReactionComponent::EndStagger() const
{
	EnemyStatsComponent->EnemyState = EEnemyState::EES_Combat; // Since enemy can be staggered only in combat (not sure about this)
	
	SSIMEnemy->StopAnimMontage();
	
	if (bShowStaggerLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Stagger ENDED (%s)"), TEXT(__FUNCTION__), *SSIMEnemy->GetName());
		// or 
		// StartStagger { OnCharacterLanded.AddDynamic; } 
		// OnCharacterLandedHandler { EndStagger; OnCharacterLanded.RemoveDynamic(); }
	}
	
	OnEndStaggerDelegate.Broadcast();
}

void USSIMEnemyDamageReactionComponent::ReboundOnHit()
{
	FVector EnemyLocation = SSIMEnemy->GetActorLocation();
	FVector PlayerLocation = DamageData.Instigator->GetActorLocation();
	FVector ReboundVelocity;
	FVector ReboundDirection;
	
	switch (PlayerAttackDirectionType)
	{
	case EPlayerAttackDirectionType::EPADT_Downward:
		{
			// Just vector down from Enemy location
			ReboundDirection = EnemyLocation + FVector::DownVector;
			ReboundVelocity = ReboundDirection * -ReboundVelocityZ;
			break;
		}
	default:
		{
			// Determine if Player is to the right or to the left
			bool bPlayerToTheRight = PlayerLocation.Y > EnemyLocation.Y;
	
			// Get unit vector from Player to Enemy and Negate that vector
			ReboundDirection = UKismetMathLibrary::NegateVector(UKismetMathLibrary::GetDirectionUnitVector(EnemyLocation, PlayerLocation));
	
			// Add rotation to that vector (around X axis)
			RotatedDirection = ReboundDirection.RotateAngleAxis(bPlayerToTheRight ? -ReboundAngle : ReboundAngle, FVector::ForwardVector);

			// Multiply by coef for launch
			ReboundVelocity = RotatedDirection * ReboundVelocityCoef;
			break;
		}
	}
	
#if !UE_BUILD_SHIPPING
	
	if (bShowReboundLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player Attack Direction Type: %s"), TEXT(__FUNCTION__),  *UEnum::GetValueAsString(PlayerAttackDirectionType));
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Rebound Velocity: %s"), TEXT(__FUNCTION__), *ReboundVelocity.ToString());
	}
	if (bDrawReboundDebug)
	{
		ReboundDrawDebug();
	}

#endif !UE_BUILD_SHIPPING
	
	SSIMEnemy->LaunchCharacter(ReboundVelocity, true, true);
}

UAnimMontage* USSIMEnemyDamageReactionComponent::SelectStaggerMontage() const
{
	TObjectPtr<UAnimMontage> StaggeredMontage;
	FVector InstigatorDirection = DamageData.Instigator->GetActorForwardVector();
	FVector EnemyDirection = SSIMEnemy->GetActorForwardVector();
	
	if (FVector::DotProduct(InstigatorDirection, EnemyDirection) > 0.f)
	{
		StaggeredMontage = BackStaggeredMontage;
	}
	else
	{
		StaggeredMontage = FrontStaggeredMontage;
	}
	return StaggeredMontage;
}

void USSIMEnemyDamageReactionComponent::ReceivePlayerAttackDirectionType(
	EPlayerAttackDirectionType InPlayerAttackDirectionType) // This value initialized by interface, not OnDamageReceived
{
	PlayerAttackDirectionType = InPlayerAttackDirectionType;
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

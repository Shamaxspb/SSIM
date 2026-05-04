// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMEnemyDamageReactionComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

//
void USSIMEnemyDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMEnemy = CastChecked<ASSIMBaseEnemy>(SSIMOwnerCharacter);
}

void USSIMEnemyDamageReactionComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	Super::OnDamageReceivedHandler(InDamageData);
	
	StartStagger();
}

void USSIMEnemyDamageReactionComponent::StartStagger()
{
	if (!IsValid(SelectStaggerMontage()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | %s montage is not valid"), 
												TEXT(__FUNCTION__), 
													 *GetNameSafe(SelectStaggerMontage()));
		return;
	}
	
	SSIMEnemy->GetEnemyStatsComponent()->EnemyState = EEnemyState::EES_Staggered;
	
	GetWorld()->GetTimerManager().SetTimer(
		StaggerTimerHandle,
		this, 
		&USSIMEnemyDamageReactionComponent::EndStagger,
		StaggerDuration,
		false
		);
	
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
	SSIMEnemy->GetEnemyStatsComponent()->EnemyState = EEnemyState::EES_Combat; // Since enemy can be staggered only in combat (not sure about this)
	
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
	switch (PlayerAttackDirectionType)
	{
	case EPlayerAttackDirectionType::EPADT_Frontal:
		{
			ASSIMPlayer* Player = Cast<ASSIMPlayer>(DamageData.Instigator);
			ReboundLaunchVelocity.Y = FrontalReboundVelocityY * Player->GetPlayerFacingDirectionValue();
			ReboundLaunchVelocity.Z = FrontalReboundVelocityZ; 
			break;	
		}
	case EPlayerAttackDirectionType::EPADT_Upward:
		{
			// Just VectorUp from Enemy location
			ReboundLaunchVelocity.Z = UpwardReboundVelocityZ;
			break;
		}
	case EPlayerAttackDirectionType::EPADT_Downward:
		{
			// Just VectorDown from Enemy location
			ReboundLaunchVelocity.Z = -DownwardReboundVelocityZ;
			break;
		}
	default:
		{
			if (bShowReboundLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Error, TEXT("%s | wtf is Player Attack Direction"), TEXT(__FUNCTION__));
			}
			break;
		}
	}
	
	Super::ReboundOnHit();
/*#if !UE_BUILD_SHIPPING
	
	if (bShowReboundLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player Attack Direction Type: %s"), TEXT(__FUNCTION__),  *UEnum::GetValueAsString(PlayerAttackDirectionType));
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Rebound Velocity: %s"), TEXT(__FUNCTION__), *ReboundLaunchVelocity.ToString());
	}
	if (bDrawReboundDebug)
	{
		ReboundDrawDebug();
	}

#endif !UE_BUILD_SHIPPING
	
	SSIMEnemy->GetCharacterMovement()->StopMovementImmediately();
	SSIMEnemy->LaunchCharacter(ReboundLaunchVelocity, true, true);
	
	// Reset value for proper next Rebound calculation 
	ReboundLaunchVelocity = FVector::ZeroVector;*/
}

UAnimMontage* USSIMEnemyDamageReactionComponent::SelectStaggerMontage() const
{
	TObjectPtr<UAnimMontage> StaggeredMontage;
	const ASSIMPlayer* Player = Cast<ASSIMPlayer>(DamageData.Instigator);
	
	if (SSIMEnemy->GetEnemyFacingDirection() == Player->GetPlayerFacingDirection())
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

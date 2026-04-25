// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMPlayerStatsComponent>();
	StatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDamageReceivedHandler);
}

// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	Super::StartAttack();
	if (bShowLogs)
    {
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack Direction: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PlayerAttackDirectionType));
    }
}

void USSIMPlayerCombatComponent::StartAttackTrace()
{	
	switch (PlayerAttackDirectionType)
	{
		case EPlayerAttackDirectionType::EPADT_Frontal:
		{
			CurrentAttackCollision = SSIMPlayer->GetFrontalAttackCollision();
			break;
		}
		
		case EPlayerAttackDirectionType::EPADT_Upward:
		{
			CurrentAttackCollision = SSIMPlayer->GetUpperAttackCollision();
			break;
		}
		
		case EPlayerAttackDirectionType::EPADT_Downward:
		{
			CurrentAttackCollision = SSIMPlayer->GetBottomAttackCollision();
			break;
		}
		
		default:
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | Couldn't receive Attack Direction to activate"), TEXT(__FUNCTION__));
			return;
		}
	}
	
	Super::StartAttackTrace();
}

void USSIMPlayerCombatComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(GetOwner());
}

UAnimMontage* USSIMPlayerCombatComponent::GetAttackMontage()
{
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		// Air Attack Montage
		switch (PlayerAttackDirectionType)
		{
		case EPlayerAttackDirectionType::EPADT_Frontal:
			{
				if (PlayerAirFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirFrontalAttackMontages[FMath::RandHelper(PlayerAirFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirectionType::EPADT_Upward:
			{
				if (PlayerAirUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirUpwardAttackMontages[FMath::RandHelper(PlayerAirUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirectionType::EPADT_Downward:
			{
				if (PlayerAirDownwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air DOWNWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirDownwardAttackMontages[FMath::RandHelper(PlayerAirDownwardAttackMontages.Num())];
				break;
			}
			
		default:
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s | Something went EXTREMELY wrong during attack animation search"), TEXT(__FUNCTION__));
				AttackMontage = nullptr;
				return nullptr;
			}
		}
	}
	else
	{
		// Ground Attack Montage
		switch (PlayerAttackDirectionType)
		{
		case EPlayerAttackDirectionType::EPADT_Frontal:
			{
				if (PlayerFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerFrontalAttackMontages[FMath::RandHelper(PlayerFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirectionType::EPADT_Upward:
			{
				if (PlayerUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerUpwardAttackMontages[FMath::RandHelper(PlayerUpwardAttackMontages.Num())];
				break;
			}
			
		default:
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s | Something went EXTREMELY wrong during attack animation search"), TEXT(__FUNCTION__));
				AttackMontage = nullptr;
				return nullptr;
			}
		}
	}
	return AttackMontage;
}

void USSIMPlayerCombatComponent::OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	HitEnemies.Add(OtherActor);
	if (bShowLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Hit Enemy : %s"), TEXT(__FUNCTION__), *OtherActor->GetName());
	}
	
	DealDamageToEnemy();
}

void USSIMPlayerCombatComponent::DealDamageToEnemy()
{
	if (HitEnemies.IsEmpty())
	{
		if (bShowLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Hit nothing"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	DamageData.Instigator = SSIMPlayer;
	DamageData.Value = RegularAttackDamage;
	
	if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward)
	{
		ReboundOnDownwardAttack();
	}
	
	for (auto Element : HitEnemies)
	{
		if (!Element->Implements<USSIMDamageableInterface>())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s : Target does not implement USSIMDamageableInterface"), TEXT(__FUNCTION__));
			return;
		}
		
		ISSIMDamageableInterface::Execute_ReceiveDamageInterface(Element, DamageData);
	}
}

void USSIMPlayerCombatComponent::ReboundOnDownwardAttack()
{
	if (FVector::DotProduct(SSIMPlayer->GetActorForwardVector(), FVector::RightVector) < 0.f)
	{
		ReboundAngle *= -1.f;
	}
	FVector ReboundDirection = SSIMPlayer->GetActorForwardVector().RotateAngleAxis(ReboundAngle,FVector::ForwardVector);
	
	ACharacter* FirstHitEnemy = Cast<ACharacter>(HitEnemies[0]);
	
	// Adjust rebound start location
	FVector AdjustedPlayerLocation = SSIMPlayer->GetActorLocation();
	AdjustedPlayerLocation.Y = FirstHitEnemy->GetActorLocation().Y;
	// Sum Player's and Enemy's capsule HalfHeights and adding a little bit 
	AdjustedPlayerLocation.Z = FirstHitEnemy->GetActorLocation().Z + 
							  (FirstHitEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() +
							  + SSIMPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
							  * 1.05; // Multiply so Player's and Enemy's capsules do not cross each other tangentially
	
	SSIMPlayer->SetActorLocation(AdjustedPlayerLocation);
	
	SSIMPlayer->LaunchCharacter(ReboundDirection * ReboundVelocityZ, true, true);
}

void USSIMPlayerCombatComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	EndAttack(); // interrupt attack to avoid stuck in attack in case of mutual attack
}

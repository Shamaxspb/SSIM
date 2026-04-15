// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Core/Interfaces/SSIMEnemyCombatInterface.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"


// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	Super::StartAttack();
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack Direction: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PlayerAttackDirection));
}

void USSIMPlayerCombatComponent::StartAttackTrace()
{	
	switch (PlayerAttackDirection)
	{
		case EPlayerAttackDirection::EPAD_Frontal:
		{
			CurrentAttackCollision = SSIMPlayer->GetFrontalAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Upward:
		{
			CurrentAttackCollision = SSIMPlayer->GetUpperAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Downward:
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
		switch (PlayerAttackDirection)
		{
		case EPlayerAttackDirection::EPAD_Frontal:
			{
				if (PlayerAirFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirFrontalAttackMontages[FMath::RandHelper(PlayerAirFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Upward:
			{
				if (PlayerAirUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirUpwardAttackMontages[FMath::RandHelper(PlayerAirUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Downward:
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
		switch (PlayerAttackDirection)
		{
		case EPlayerAttackDirection::EPAD_Frontal:
			{
				if (PlayerFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerFrontalAttackMontages[FMath::RandHelper(PlayerFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Upward:
			{
				if (PlayerUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerUpwardAttackMontages[FMath::RandHelper(PlayerUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Downward:
			{
				if (PlayerDownwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No DOWNWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerDownwardAttackMontages[FMath::RandHelper(PlayerDownwardAttackMontages.Num())];
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
	Super::OnAttackCollisionBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	DealDamageToEnemy();
}

void USSIMPlayerCombatComponent::DealDamageToEnemy()
{
	for (auto Element : HitCharacters)
	{
		if (!Element->Implements<USSIMEnemyCombatInterface>())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s : Target does not implement USSIMEnemyCombatInterface"), TEXT(__FUNCTION__));
			return;
		}
		
		ISSIMEnemyCombatInterface::Execute_ReceiveDamage(Element, RegularAttackDamage);
	}
}


//DEBUG
void USSIMPlayerCombatComponent::SwitchAttackCollision_DEBUG() const
{
	if (!IsValid(CurrentAttackCollision))
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("No current attack collision"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, 
										TEXT("No current attack collision"), false, FVector2D(1.2f, 1.2f));
		return;
	}

	
	
	if (CurrentAttackCollision->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	{
		CurrentAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CurrentAttackCollision->SetHiddenInGame(true);
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Current attack collision: NoCollision"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Red, 
										TEXT("Current attack collision: NoCollision"), false, FVector2D(1.2f, 1.2f));
	}
	else if (CurrentAttackCollision->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		CurrentAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CurrentAttackCollision->SetHiddenInGame(false);
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Current attack collision: QueryOnly"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Emerald, 
										TEXT("Current attack collision: QueryOnly"), false, FVector2D(1.2f, 1.2f));
	}
}

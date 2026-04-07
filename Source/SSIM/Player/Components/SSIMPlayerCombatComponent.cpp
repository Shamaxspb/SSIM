// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "SSIM/Player/SSIMPlayer.h"

// Overriden Functions
USSIMPlayerCombatComponent::USSIMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void USSIMPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	if (bIsAttacking)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack is still in process"));
		return;
	}
	
	if (!IsValid(GetAttackMontage()))
	{
		return;
	}
	UAnimMontage* AttackMontage = GetAttackMontage();
	
	bIsAttacking = true; // set to false in ANS_NotifyEnd, so ANS MUST be in AnimMontage
	
	SSIMAnimInstance->Montage_Play(AttackMontage);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack Montage: %s"), *AttackMontage->GetName());
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack started"));	
}


void USSIMPlayerCombatComponent::StartAttackFrontal()
{
	PlayerAttackDirection = EPlayerAttackDirection::EPAD_Frontal;
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack FRONTAL"));
	StartAttack();
}


void USSIMPlayerCombatComponent::StartAttackUpward()
{
	PlayerAttackDirection = EPlayerAttackDirection::EPAD_Upwards;
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack UPWARD"));
	StartAttack();
}


void USSIMPlayerCombatComponent::StartAttackDownward()
{
	PlayerAttackDirection = EPlayerAttackDirection::EPAD_Downwards;
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack DOWNWARD"));
	StartAttack();
}


void USSIMPlayerCombatComponent::DamageProcessing()
{
	if (!IsValid(CurrentAttackCollision))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("No Valid Attack Collision"))
		return;
	}
	
	// CachedAttackCollision->OnComponentBeginOverlap
}


void USSIMPlayerCombatComponent::EndAttack()
{
	bIsAttacking = false;
	DeactivateAttackCollision();
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack ended"));
}


UAnimMontage* USSIMPlayerCombatComponent::GetAttackMontage() const
{
	UAnimMontage* AttackMontage;
	
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		// Air Attack Montage
		switch (PlayerAttackDirection)
		{
		case EPlayerAttackDirection::EPAD_Frontal:
			{
				if (PlayerAirFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("No AIR FRONTAL Attack Montages found"));
					return nullptr;
				}
				AttackMontage = PlayerAirFrontalAttackMontages[FMath::RandHelper(PlayerAirFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Upwards:
			{
				if (PlayerAirUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("No AIR UPWARD Attack Montages found"));
					return nullptr;
				}
				AttackMontage = PlayerAirUpwardAttackMontages[FMath::RandHelper(PlayerAirUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Downwards:
			{
				if (PlayerAirDownwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("No Air DOWNWARD Attack Montages found"));
					return nullptr;
				}
				AttackMontage = PlayerAirDownwardAttackMontages[FMath::RandHelper(PlayerAirDownwardAttackMontages.Num())];
				break;
			}
			
		default:
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("Something went EXTREMELY wrong during attack animation search"));
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
					UE_LOG(LogSSIMValidations, Error, TEXT("No FRONTAL Attack Montages found"));
					return nullptr;
				}
				AttackMontage = PlayerFrontalAttackMontages[FMath::RandHelper(PlayerFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Upwards:
			{
				if (PlayerUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("No UPWARD Attack Montages found"));
					return nullptr;
				}
				AttackMontage = PlayerUpwardAttackMontages[FMath::RandHelper(PlayerUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Downwards:
			{
				if (PlayerDownwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("No DOWNWARD Attack Montages found"));
					return nullptr;
				}
				AttackMontage = PlayerDownwardAttackMontages[FMath::RandHelper(PlayerDownwardAttackMontages.Num())];
				break;
			}
			
		default:
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("Something went EXTREMELY wrong during attack animation search"));
				AttackMontage = nullptr;
				return nullptr;
			}
		}
	}
	return AttackMontage;
}


void USSIMPlayerCombatComponent::ActivateAttackCollision()
{
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("AttackDirection: %s"), *UEnum::GetValueAsString(PlayerAttackDirection));
	
	switch (PlayerAttackDirection)
	{
		case EPlayerAttackDirection::EPAD_Frontal:
		{
			CurrentAttackCollision = SSIMPlayer->GetFrontalAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Upwards:
		{
			CurrentAttackCollision = SSIMPlayer->GetUpperAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Downwards:
		{
			CurrentAttackCollision = SSIMPlayer->GetBottomAttackCollision();
			break;
		}
		
		default:
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("Couldn't receive Attack Direction to activate"));
			return;
		}
	}
	
	CurrentAttackCollision->SetActive(true);
	CurrentAttackCollision->SetHiddenInGame(false);
}


void USSIMPlayerCombatComponent::DeactivateAttackCollision() const
{
	if (!CurrentAttackCollision)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("Couldn't receive Attack Collision to deactivate"))
		return;
	}
	
	CurrentAttackCollision->SetActive(false);
	CurrentAttackCollision->SetHiddenInGame(true);
}

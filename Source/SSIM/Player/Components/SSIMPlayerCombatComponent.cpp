// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"
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
	
	UAnimMontage* AttackMontage;
	bIsAttacking = true; // set to false in ANS, so it MUST be in AnimMontage

	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		if (!IsValid(PlayerAirAttackMontage))
		{
			UE_LOG(LogSSIMGameplayMessages, Error, TEXT("No Air Attack montage found"));
			return;
		}
		AttackMontage = PlayerAirAttackMontage;
	}
	else
	{
		if (PlayerAttackAnimations.IsEmpty())
		{
			UE_LOG(LogSSIMGameplayMessages, Error, TEXT("No Attack montages found"));
			return;
		}
		AttackMontage = PlayerAttackAnimations[FMath::RandHelper(PlayerAttackAnimations.Num())];
	}
	
	SSIMAnimInstance->Montage_Play(AttackMontage);
	//UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack Montage: %s"), *RandomAttackMontage->GetName());
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack started"));	
}

void USSIMPlayerCombatComponent::EndAttack()
{
	bIsAttacking = false;
	DeactivateAttackCollision();
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Attack ended"));
}

void USSIMPlayerCombatComponent::ActivateAttackCollision(EPlayerAttackDirection InPlayerAttackDirection)
{
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("AttackDirection: %s"), *UEnum::GetValueAsString(InPlayerAttackDirection));
	
	switch (InPlayerAttackDirection)
	{
		case EPlayerAttackDirection::EPAD_Frontal:
		{
			CachedAttackCollision = SSIMPlayer->GetFrontalAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Upwards:
		{
			CachedAttackCollision = SSIMPlayer->GetUpperAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Downwards:
		{
			CachedAttackCollision = SSIMPlayer->GetBottomAttackCollision();
			break;
		}
		
		default:
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("Couldn't receive Attack Direction to activate"));
			return;
		}
	}
	
	CachedAttackCollision->SetActive(true);
	CachedAttackCollision->SetHiddenInGame(false);
	
}

void USSIMPlayerCombatComponent::DeactivateAttackCollision() const
{
	if (!CachedAttackCollision)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("Couldn't receive Attack Collision to deactivate"))
		return;
	}
	
	CachedAttackCollision->SetActive(false);
	CachedAttackCollision->SetHiddenInGame(true);

}

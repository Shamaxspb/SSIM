// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"

#include "Components/ArrowComponent.h"
#include "SSIM/SSIM.h"

// Overriden Functions
ASSIMPlayer::ASSIMPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentPlayerState = EPlayerState::EPS_Movement;
	
	SSIMPlayerCombatComponent = CreateDefaultSubobject<USSIMPlayerCombatComponent>("PlayerCombatComponent");
	SSIMPlayerFlowComponent = CreateDefaultSubobject<USSIMPlayerFlowComponent>("PlayerFlowComponent");
}

void ASSIMPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetArrowComponent()->SetHiddenInGame(false);
	
}

void ASSIMPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASSIMPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


// My Functions


// Interfaces
// This entire thing is just practice, and it is an overengineering, just get all this data in ActorComponent directly
UAnimMontage* ASSIMPlayer::GetDashAnimationInterface()
{
	return PlayerDashAnimation;
}

TArray<UAnimMontage*> ASSIMPlayer::GetAttackAnimationsInterface()
{
	return PlayerAttackAnimations;
}

UAnimMontage* ASSIMPlayer::GetRandomAttackAnimationInterface()
{
	if (PlayerAttackAnimations.IsEmpty())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("PlayerAttackAnimations array is empty"));
		return nullptr;
	}
	
	return PlayerAttackAnimations[FMath::RandHelper(PlayerAttackAnimations.Num())];
}
// ------------------ end of overengineering

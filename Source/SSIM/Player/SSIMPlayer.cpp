// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"
#include "EnhancedInputComponent.h"
#include "Components/ArrowComponent.h"
#include "SSIM/SSIM.h"

// Overriden Functions
ASSIMPlayer::ASSIMPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentPlayerState = EPlayerState::EPS_Movement;
	
	SSIMPlayerCombatComponent	= CreateDefaultSubobject<USSIMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	SSIMPlayerFlowComponent		= CreateDefaultSubobject<USSIMPlayerFlowComponent>(TEXT("PlayerFlowComponent"));
	SSIMPlayerInputComponent	= CreateDefaultSubobject<UEnhancedInputComponent>(TEXT("PlayerInputComponent"));
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
	
	if (!PlayerInputComponent)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("PlayerInputComponent is not valid"));
	}
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//Input->BindAction()
	
}


// My Functions


// Interfaces
// This entire thing is just practice, and it is an overengineering, just get all this data in ActorComponent directly
TArray<UAnimMontage*> ASSIMPlayer::GetAttackAnimationsInterface_Implementation() const
{
	return PlayerAttackAnimations;
}

UAnimMontage* ASSIMPlayer::GetRandomAttackAnimationInterface_Implementation() const
{
	if (PlayerAttackAnimations.IsEmpty())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("PlayerAttackAnimations array is empty"));
		return nullptr;
	}
	
	return PlayerAttackAnimations[FMath::RandHelper(PlayerAttackAnimations.Num())];
}
// ------------------ end of overengineering

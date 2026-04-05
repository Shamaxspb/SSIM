// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"
#include "EnhancedInputComponent.h"
#include "SSIMPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "SSIM/SSIM.h"

// Overriden Functions
ASSIMPlayer::ASSIMPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentPlayerState = EPlayerState::EPS_Movement;
	
	SSIMPlayerCombatComponent	= CreateDefaultSubobject<USSIMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	SSIMPlayerFlowComponent		= CreateDefaultSubobject<USSIMPlayerFlowComponent>(TEXT("PlayerFlowComponent"));

	SetupAttackCollision();
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
		return;
	}
	
	UEnhancedInputComponent* SSIMInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (!IsValid(SSIMInputComponent))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("EnhancedInputComponent is not valid. Or cast failed"));
		return;
	}
		
	SSIMInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ASSIMPlayer::MoveRight);
	SSIMInputComponent->BindAction(MoveLeftInputAction,  ETriggerEvent::Triggered, this, &ASSIMPlayer::MoveLeft);
	SSIMInputComponent->BindAction(DashInputAction,		 ETriggerEvent::Started,   this, &ASSIMPlayer::HandleDash);
	SSIMInputComponent->BindAction(AttackInputAction,	 ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttack);
	
	
}



// My Functions
void ASSIMPlayer::MoveRight()
{
	if (SSIMPlayerFlowComponent->bDashing)
	{
		//UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Dash is in process, cannot move"));
		return;
	}
	
	AddMovementInput(FVector::RightVector, 1.f, false);
	SetActorRotation(FRotator(0, 90, 0));
}

void ASSIMPlayer::MoveLeft()
{
	if (SSIMPlayerFlowComponent->bDashing)
	{
		//UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Dash is in process, cannot move"));
		return;
	}
	
	AddMovementInput(FVector::RightVector * -1.f, 1.f, false);
	SetActorRotation(FRotator(0, -90, 0));
}

void ASSIMPlayer::HandleDash()
{
	SSIMPlayerFlowComponent->Dash();
}

void ASSIMPlayer::HandleAttack()
{
	SSIMPlayerCombatComponent->StartAttack();
}

void ASSIMPlayer::SetupAttackCollision()
{
	FrontalAttackCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("FrontalAttackCollision"));
	
	
	UpperAttackCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("UpperAttackCollision"));
	
	
	BottomAttackCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("BottomAttackCollision"));

}


// Interfaces
USSIMPlayerCombatComponent* ASSIMPlayer::GetPlayerCombatComponentInterface_Implementation() const
{
	return SSIMPlayerCombatComponent;
}

USSIMPlayerFlowComponent* ASSIMPlayer::GetPlayerFlowComponentInterface_Implementation() const
{
	return SSIMPlayerFlowComponent;
}

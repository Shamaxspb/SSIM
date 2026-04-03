// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"
#include "EnhancedInputComponent.h"
#include "SSIMPlayerController.h"
#include "Components/ArrowComponent.h"
#include "SSIM/SSIM.h"

// Overriden Functions
ASSIMPlayer::ASSIMPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentPlayerState = EPlayerState::EPS_Movement;
	
	SSIMPlayerCombatComponent	= CreateDefaultSubobject<USSIMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	SSIMPlayerFlowComponent		= CreateDefaultSubobject<USSIMPlayerFlowComponent>(TEXT("PlayerFlowComponent"));
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



// Interfaces
USSIMPlayerCombatComponent* ASSIMPlayer::GetPlayerCombatComponentInterface_Implementation() const
{
	return SSIMPlayerCombatComponent;
}

USSIMPlayerFlowComponent* ASSIMPlayer::GetPlayerFlowComponentInterface_Implementation() const
{
	return SSIMPlayerFlowComponent;
}

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

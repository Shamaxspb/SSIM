// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"
#include "SSIM/SSIM.h"
#include "EnhancedInputComponent.h"
#include "SSIMPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SSIMPlayerCombatComponent.h"
#include "Components/SSIMPlayerFlowComponent.h"

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
		
	SSIMInputComponent->BindAction(MoveRightInputAction,		ETriggerEvent::Triggered, this, &ASSIMPlayer::MoveRight);
	SSIMInputComponent->BindAction(MoveLeftInputAction,			ETriggerEvent::Triggered, this, &ASSIMPlayer::MoveLeft);
	SSIMInputComponent->BindAction(DashInputAction,				ETriggerEvent::Started,   this, &ASSIMPlayer::HandleDash);
	SSIMInputComponent->BindAction(AttackInputAction,			ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttackFrontal);
	SSIMInputComponent->BindAction(AttackUpwardInputAction,		ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttackUpward);
	SSIMInputComponent->BindAction(AttackDownwardInputAction,	ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttackDownward);
}



// My Functions
void ASSIMPlayer::MoveRight()
{
	if (SSIMPlayerFlowComponent->bDashing)
	{
		return;
	}
	
	AddMovementInput(FVector::RightVector, 1.f, false);
	SetActorRotation(FRotator(0, 90, 0));
}

void ASSIMPlayer::MoveLeft()
{
	if (SSIMPlayerFlowComponent->bDashing)
	{
		return;
	}
	
	AddMovementInput(FVector::RightVector * -1.f, 1.f, false);
	SetActorRotation(FRotator(0, -90, 0));
}

void ASSIMPlayer::HandleDash()
{
	SSIMPlayerFlowComponent->Dash();
}

void ASSIMPlayer::HandleAttackFrontal()
{
	SSIMPlayerCombatComponent->StartAttackFrontal();
}

void ASSIMPlayer::HandleAttackUpward()
{
	SSIMPlayerCombatComponent->StartAttackUpward();
}

void ASSIMPlayer::HandleAttackDownward()
{
	SSIMPlayerCombatComponent->StartAttackDownward();
}

void ASSIMPlayer::SetupAttackCollision()
{
	RootAttackCollisionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AttackCollisionRoot"));
	RootAttackCollisionComponent->SetupAttachment(GetRootComponent());
	
	FrontalAttackCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("FrontalAttackCollision"));
	UpperAttackCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("UpperAttackCollision"));
	BottomAttackCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("BottomAttackCollision"));
	
	TArray<TObjectPtr<UBoxComponent>> AttackCollisions;
	AttackCollisions.Add(FrontalAttackCollision);
	AttackCollisions.Add(UpperAttackCollision);
	AttackCollisions.Add(BottomAttackCollision);

	for (auto const Element : AttackCollisions)
	{
		Element->SetupAttachment(RootAttackCollisionComponent);
		Element->SetActive(false);
		Element->SetCollisionProfileName("AttackTrace", true);
	}
	
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

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
	
	SSIMPlayerCombatComponent = CreateDefaultSubobject<USSIMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	SSIMPlayerFlowComponent	  = CreateDefaultSubobject<USSIMPlayerFlowComponent>(TEXT("PlayerFlowComponent"));

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
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s PlayerInputComponent is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	UEnhancedInputComponent* SSIMInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (!IsValid(SSIMInputComponent))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s : EnhancedInputComponent is not valid. Or cast failed"), TEXT(__FUNCTION__));
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
		Element->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Element->SetGenerateOverlapEvents(true);
		Element->SetCollisionProfileName("AttackTrace", true);
	}
	
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

void ASSIMPlayer::HandleStartAttackTrace()
{
	SSIMPlayerCombatComponent->StartAttackTrace();
}

void ASSIMPlayer::HandleEndAttackTrace()
{
	SSIMPlayerCombatComponent->EndAttackTrace();

}

void ASSIMPlayer::HandleDash()
{
	SSIMPlayerFlowComponent->Dash();
}

// Interfaces
void ASSIMPlayer::StartAttackTraceInterface_Implementation() const
{
	SSIMPlayerCombatComponent->StartAttackTrace();
}

void ASSIMPlayer::EndAttackTraceInterface_Implementation() const
{
	SSIMPlayerCombatComponent->EndAttackTrace();
}

void ASSIMPlayer::EndAttackInterface_Implementation() const
{
	SSIMPlayerCombatComponent->EndAttack();
}

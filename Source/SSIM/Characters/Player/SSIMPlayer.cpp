// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"

#include "SSIM/SSIM.h"
#include "EnhancedInputComponent.h"
#include "SSIMPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerDamageReactionComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerFlowComponent.h"

// Overriden Functions
ASSIMPlayer::ASSIMPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName("Player", true);
	GetMesh()->SetCollisionProfileName("Player", true);
	
	SSIMPlayerFlowComponent	  		  = CreateDefaultSubobject<USSIMPlayerFlowComponent>(TEXT("PlayerFlowComponent"));
	SSIMPlayerCombatComponent 		  = CreateDefaultSubobject<USSIMPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	SSIMPlayerStatsComponent  		  = CreateDefaultSubobject<USSIMPlayerStatsComponent>(TEXT("PlayerStatsComponent"));
	SSIMPlayerDamageReactionComponent = CreateDefaultSubobject<USSIMPlayerDamageReactionComponent>(TEXT("PlayerDamageReactionComponent"));

	SetupAttackCollision();
	
	
}

void ASSIMPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->GravityScale = DEFAULT_GRAVITY_SCALE;
	
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
	if (SSIMPlayerFlowComponent->bDashing || SSIMPlayerStatsComponent->bStaggered)
	{
		return;
	}
	
	AddMovementInput(FVector::RightVector, 1.f, false);
	SetActorRotation(FRotator(0, 90, 0));
}

void ASSIMPlayer::MoveLeft()
{
	if (SSIMPlayerFlowComponent->bDashing || SSIMPlayerStatsComponent->bStaggered)
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
		Element->SetGenerateOverlapEvents(true);
		Element->SetCollisionProfileName("MeleeAttack", true);
	}
	
}

void ASSIMPlayer::HandleAttackFrontal()
{
	SSIMPlayerCombatComponent->PlayerAttackDirection = EPlayerAttackDirection::EPAD_Frontal;
	SSIMPlayerCombatComponent->StartAttack();
}

void ASSIMPlayer::HandleAttackUpward()
{
	SSIMPlayerCombatComponent->PlayerAttackDirection = EPlayerAttackDirection::EPAD_Upward;
	SSIMPlayerCombatComponent->StartAttack();
}

void ASSIMPlayer::HandleAttackDownward()
{
	SSIMPlayerCombatComponent->PlayerAttackDirection = EPlayerAttackDirection::EPAD_Downward;
	SSIMPlayerCombatComponent->StartAttack();
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
	SSIMPlayerFlowComponent->StartDash();
}


// Interfaces
void ASSIMPlayer::StartAttackInterface_Implementation() const
{
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s | Shouldn't call player's Start Attack from outside player related classes"), TEXT(__FUNCTION__));
}

void ASSIMPlayer::EndAttackInterface_Implementation() const
{
	SSIMPlayerCombatComponent->EndAttack();
}

void ASSIMPlayer::StartAttackTraceInterface_Implementation() const
{
	SSIMPlayerCombatComponent->StartAttackTrace();
}

void ASSIMPlayer::EndAttackTraceInterface_Implementation() const
{
	SSIMPlayerCombatComponent->EndAttackTrace();
}


void ASSIMPlayer::EndDashInterface_Implementation() const
{
	SSIMPlayerFlowComponent->EndDash();
}


void ASSIMPlayer::ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const
{
	if (SSIMPlayerStatsComponent->bInvulnerable)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player is invulnerable"), TEXT(__FUNCTION__));
		return;
	}
	SSIMPlayerStatsComponent->ReduceHealth(InDamageData);
}
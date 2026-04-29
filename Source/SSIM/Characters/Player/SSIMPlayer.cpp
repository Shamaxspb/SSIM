// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayer.h"

#include "SSIM/SSIM.h"
#include "EnhancedInputComponent.h"
#include "SSIMPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/DamageReaction//SSIMPlayerDamageReactionComponent.h"
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
	
	SetPlayerGravityScaleToDefault();
	SetPlayerBrakingDecelerationWalkingToDefault();
	SetContactDamageCollisionShapeDefault();
}

void ASSIMPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->GravityScale = DefaultPlayerGravityScale;
	
	BindToStateChangesInComponents();
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
	if (CanMove())
	{
		AddMovementInput(FVector::RightVector, 1.f, false);
		SetActorRotation(FRotator(0, 90, 0));
	}
}

void ASSIMPlayer::MoveLeft()
{
	if (CanMove())
	{
		AddMovementInput(FVector::RightVector * -1.f, 1.f, false);
		SetActorRotation(FRotator(0, -90, 0));
	}
	
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

void ASSIMPlayer::BindToStateChangesInComponents() const
{
	SSIMPlayerCombatComponent->OnAttackStartedDelegate.AddDynamic(this, &ASSIMPlayer::OnAttackStartedHandler);
	SSIMPlayerCombatComponent->OnAttackEndedDelegate.AddDynamic(this, &ASSIMPlayer::OnAttackEndedHandler);
	
	SSIMPlayerCombatComponent->OnPogoStartedDelegate.AddDynamic(this, &ASSIMPlayer::OnPogoStartedHandler);
	SSIMPlayerCombatComponent->OnPogoEndedDelegate.AddDynamic(this, &ASSIMPlayer::OnPogoEndedHandler);
	
	SSIMPlayerFlowComponent->OnDashStartedDelegate.AddDynamic(this, &ASSIMPlayer::OnDashStartedHandler);
	SSIMPlayerFlowComponent->OnDashEndedDelegate.AddDynamic(this, &ASSIMPlayer::OnDashEndedHandler);
	
	SSIMPlayerFlowComponent->OnCanDashChangedDelegate.AddDynamic(this,&ASSIMPlayer::OnCanDashStateChangedHandler);
	
	SSIMPlayerDamageReactionComponent->OnStaggerStartedDelegate.AddDynamic(this, &ASSIMPlayer::OnStaggerStartedHandler);
	SSIMPlayerDamageReactionComponent->OnStaggerEndedDelegate.AddDynamic(this, &ASSIMPlayer::OnStaggerEndedHandler);
	
}

void ASSIMPlayer::HandleAttackFrontal()
{
	if (CanAttack())
	{
		SSIMPlayerCombatComponent->PlayerAttackDirectionType = EPlayerAttackDirectionType::EPADT_Frontal;
        SSIMPlayerCombatComponent->StartAttack();
	}
}

void ASSIMPlayer::HandleAttackUpward()
{
	if (CanAttack())
	{
		SSIMPlayerCombatComponent->PlayerAttackDirectionType = EPlayerAttackDirectionType::EPADT_Upward;
		SSIMPlayerCombatComponent->StartAttack();
	}
}

void ASSIMPlayer::HandleAttackDownward()
{
	if (CanAttack())
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Cannot attack downwards from the ground. Frontal Attack used instead"), TEXT(__FUNCTION__));
			}
		
			SSIMPlayerCombatComponent->PlayerAttackDirectionType = EPlayerAttackDirectionType::EPADT_Frontal;
			SSIMPlayerCombatComponent->StartAttack();
			return;
		}
		SSIMPlayerCombatComponent->PlayerAttackDirectionType = EPlayerAttackDirectionType::EPADT_Downward;
		SSIMPlayerCombatComponent->StartAttack();
	}
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
	if (bPogoActive || bDashing || !bCanDash || bStaggered)
	{
		if (bPogoActive)
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Pogo is still in process"), TEXT(__FUNCTION__));
			}
			return;
		}
		if (bDashing)
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is still in process"), TEXT(__FUNCTION__));
			}
		}
		if (!bCanDash)
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is on cooldown for %f"), TEXT(__FUNCTION__), GetWorld()->GetTimerManager().GetTimerRemaining(SSIMPlayerFlowComponent->DashCooldownTimerHandle));
			}
			return;	
		}
		if (bStaggered)
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Can't Dash during stagger"), TEXT(__FUNCTION__));
			}
			return;	
		}
	}
	SSIMPlayerFlowComponent->StartDash();
}

#pragma region State Handlers
void ASSIMPlayer::OnAttackStartedHandler()
{
	bAttacking = true;
}
void ASSIMPlayer::OnAttackEndedHandler()
{
	bAttacking = false;
}

void ASSIMPlayer::OnPogoStartedHandler()
{
	bPogoActive = true;
}
void ASSIMPlayer::OnPogoEndedHandler()
{
	bPogoActive = false;
}

void ASSIMPlayer::OnDashStartedHandler()
{
	bDashing = true;
}
void ASSIMPlayer::OnDashEndedHandler()
{
	bDashing = false;
}

void ASSIMPlayer::OnCanDashStateChangedHandler(bool InCanDash)
{
	bCanDash = InCanDash;
}

void ASSIMPlayer::OnStaggerStartedHandler()
{
	bStaggered = true;
}

void ASSIMPlayer::OnStaggerEndedHandler()
{
	bStaggered = false;
}
#pragma endregion State Handlers

bool ASSIMPlayer::CanAttack() const
{
	if (bAttacking || bDashing)
	{
		if (bAttacking)
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack is in process"), TEXT(__FUNCTION__));
			}
		}
		if (bDashing)
		{
			if (bShowLogs)
			{
				UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Dash is in process"), TEXT(__FUNCTION__));
			}
		}
		return false;
	}
	return true;
}

bool ASSIMPlayer::CanMove() const
{
	if (/*bPogoActive || */bDashing || bStaggered)
	{
		return false;
	}
	return true;
}


#pragma region Interfaces
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
	SSIMPlayerStatsComponent->ReduceHealth(InDamageData);
}
#pragma endregion Interfaces

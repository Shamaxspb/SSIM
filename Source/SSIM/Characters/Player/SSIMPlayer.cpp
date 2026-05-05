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
#include "SSIM/Components/PlayerComponents/SSIMPlayerDashComponent.h"

// Overriden Functions
ASSIMPlayer::ASSIMPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName("Player", true);
	GetMesh()->SetCollisionProfileName("Player", true);
	
	SSIMPlayerDashComponent	  		  = CreateDefaultSubobject<USSIMPlayerDashComponent>(TEXT("PlayerDashComponent"));
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
	
	PlayerFacingDirection = FVector::DotProduct(GetActorForwardVector(), FVector::RightVector) > 0.f ?
												EFacingDirection::EPD_Right : EFacingDirection::EPD_Left;
	
	SSIMPlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &ASSIMPlayer::OnDamageReceivedHandler);
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
		
	SSIMInputComponent->BindAction(MoveInputAction,				ETriggerEvent::Triggered, this, &ASSIMPlayer::HandleMove);
	SSIMInputComponent->BindAction(MoveInputAction,				ETriggerEvent::Completed, this, &ASSIMPlayer::HandleMoveCompleted);
	
	SSIMInputComponent->BindAction(DashInputAction,				ETriggerEvent::Started,   this, &ASSIMPlayer::HandleDash);
	
	SSIMInputComponent->BindAction(AttackInputAction,			ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttackFrontal);
	SSIMInputComponent->BindAction(AttackUpwardInputAction,		ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttackUpward);
	SSIMInputComponent->BindAction(AttackDownwardInputAction,	ETriggerEvent::Started,   this, &ASSIMPlayer::HandleAttackDownward);
	
	SSIMInputComponent->BindAction(HealInputAction,				ETriggerEvent::Started,   this, &ASSIMPlayer::HandleHeal);
}

// My Functions
void ASSIMPlayer::SetPlayerFacingDirection(EFacingDirection InPlayerFacingDirection)
{
	PlayerFacingDirection = InPlayerFacingDirection;
	FRotator PlayerRotation = GetActorRotation();
	switch (InPlayerFacingDirection)
	{
	case EFacingDirection::EPD_Right:
		{
			PlayerRotation.Yaw = 90.f;
			break;
		}
	case EFacingDirection::EPD_Left:
		{
			PlayerRotation.Yaw = -90.f;
			break;
		}
	}
	SetActorRotation(PlayerRotation);
}

void ASSIMPlayer::HandleMove(const FInputActionValue& Value)
{
	if (CanMove())
	{
		MoveInputValue = Value.Get<float>();

		float CachedInputValue = Value.Get<float>();
		
		// Move Right
 		if (Value.Get<float>() > 0.f)
		{
			CachedInputValue = FMath::Clamp(MoveInputValue,1.f, 1.f);
 			SetPlayerFacingDirection(EFacingDirection::EPD_Right);
		}
		// Move Left
		else if (Value.Get<float>() < 0.f)
		{
			CachedInputValue = FMath::Clamp(MoveInputValue,-1.f, -1.f);
			SetPlayerFacingDirection(EFacingDirection::EPD_Left);
		}
		// Do not move
		else
		{
			MoveInputValue = 0.f;
		}
		
		AddMovementInput(FVector::RightVector, CachedInputValue, false);
	}
}

void ASSIMPlayer::HandleMoveCompleted()
{
	MoveInputValue = 0.f;
	if (SSIMPlayerDamageReactionComponent->bStaggered || SSIMPlayerDashComponent->bDashing)
	{
		return;
	}
	GetCharacterMovement()->Velocity.Y = 0.f;
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
			if (bShowPlayerInputLogs)
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

void ASSIMPlayer::HandleStartAttackTrace() const
{
	SSIMPlayerCombatComponent->StartAttackTrace();
}

void ASSIMPlayer::HandleEndAttackTrace() const
{
	SSIMPlayerCombatComponent->EndAttackTrace();

}

void ASSIMPlayer::HandleHeal()
{
	if (CanHeal())
	{
		SSIMPlayerStatsComponent->StartHealing();
	}
}

void ASSIMPlayer::HandleDash()
{
	if (CanDash())
	{
		SSIMPlayerDashComponent->StartDash();
	}
}

void ASSIMPlayer::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	MoveInputValue = 0.f;
}

bool ASSIMPlayer::CanMove() const
{
	if (SSIMPlayerDashComponent->bDashing 
		|| 
		SSIMPlayerDamageReactionComponent->bStaggered 
		|| 
		SSIMPlayerCombatComponent->bAttackKnockbackActive
		||
		SSIMPlayerStatsComponent->bHealing)
	{
		return false;
	}
	return true;
}

bool ASSIMPlayer::CanDash() const
{
	if (SSIMPlayerDashComponent->bDashing 
		|| 
		!SSIMPlayerDashComponent->bCanDash 
		|| 
		SSIMPlayerDamageReactionComponent->bStaggered)
	{
		if (SSIMPlayerDashComponent->bDashing)
		{
			if (bShowPlayerInputLogs)
			{
				UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Dash is still in process"), TEXT(__FUNCTION__));
			}
		}
		if (!SSIMPlayerDashComponent->bCanDash)
		{
			if (bShowPlayerInputLogs)
			{
				UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Dash is on cooldown for %f"), TEXT(__FUNCTION__), GetWorld()->GetTimerManager().GetTimerRemaining(SSIMPlayerDashComponent->DashCooldownTimerHandle));
			}
		}
		if (SSIMPlayerDamageReactionComponent->bStaggered)
		{
			if (bShowPlayerInputLogs)
			{
				UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Can't Dash while staggered"), TEXT(__FUNCTION__));
			}
		}
		return false;
	}
	
	return true;
}

bool ASSIMPlayer::CanAttack() const
{
	if (SSIMPlayerCombatComponent->bAttacking 
		|| 
		SSIMPlayerDashComponent->bDashing
		||
		SSIMPlayerDamageReactionComponent->bStaggered)
	{
		if (SSIMPlayerCombatComponent->bAttacking)
		{
			if (bShowPlayerInputLogs)
			{
				UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Attack is in process"), TEXT(__FUNCTION__));
			}
		}
		if (SSIMPlayerDashComponent->bDashing)
		{
			if (bShowPlayerInputLogs)
			{
				UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Dash is in process"), TEXT(__FUNCTION__));
			}
		}
		if (SSIMPlayerDamageReactionComponent->bStaggered)
		{
			if (bShowPlayerInputLogs)
			{
				UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Can't Attack while staggered"), TEXT(__FUNCTION__));
			}
		}
		return false;
	}
	return true;
}

bool ASSIMPlayer::CanHeal() const
{
	if (SSIMPlayerDamageReactionComponent->bStaggered)
	{
		if (SSIMPlayerDamageReactionComponent->bStaggered)
		{
			UE_LOG(LogSSIMInputValidation, Warning, TEXT("%s | Can't Heal while staggered"), TEXT(__FUNCTION__));
		}
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
	SSIMPlayerDashComponent->EndDash();
}


void ASSIMPlayer::ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const
{
	SSIMPlayerStatsComponent->ReduceHealth(InDamageData);
}
#pragma endregion Interfaces                                                                                                       

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerFlowComponent.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

USSIMPlayerCombatComponent::USSIMPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

// Overriden Functions
void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDamageReceivedHandler);
	PlayerFlowComponent->OnDashStartedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDashStartedHandler);
	
	EndPogoTimerDelegate.BindUObject(this, &USSIMPlayerCombatComponent::EndPogo);
}

void USSIMPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (SSIMPlayer->GetIsPlayerAttackKnockbackActive() && AttackKnockbackType == EAttackKnockbackType::EAKT_Ground)
	{
		//SSIMPlayer->GetCharacterMovement()->Velocity.Z = 0.f;
		SSIMPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	Super::StartAttack();
	
	if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward)
	{
		FOnMontageEnded OnPogoAnimationEnded;
		OnPogoAnimationEnded.BindUObject(this, &USSIMPlayerCombatComponent::PogoAnimationCallback);
		
		OnPogoAnimationStartedDelegate.Broadcast();
		SSIMPlayer->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnPogoAnimationEnded, AttackMontage);
	}
	
	if (bShowAttackLogs)
    {
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Player Attack Direction: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PlayerAttackDirectionType));
    }
}

void USSIMPlayerCombatComponent::StartAttackTrace()
{	
	switch (PlayerAttackDirectionType)
	{
		case EPlayerAttackDirectionType::EPADT_Frontal:
		{
			CurrentAttackCollision = SSIMPlayer->GetFrontalAttackCollision();
			break;
		}
		
		case EPlayerAttackDirectionType::EPADT_Upward:
		{
			CurrentAttackCollision = SSIMPlayer->GetUpperAttackCollision();
			break;
		}
		
		case EPlayerAttackDirectionType::EPADT_Downward:
		{
			CurrentAttackCollision = SSIMPlayer->GetBottomAttackCollision();
			break;
		}
		
		default:
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s | Couldn't receive Attack Direction to activate"), TEXT(__FUNCTION__));
			return;
		}
	}
	
	Super::StartAttackTrace();
}

void USSIMPlayerCombatComponent::SetReferences()
{
	Super::SetReferences();
	
	SSIMPlayer = CastChecked<ASSIMPlayer>(GetOwner());
	PlayerStatsComponent = SSIMPlayer->GetPlayerStatsComponent();
	PlayerFlowComponent  = SSIMPlayer->GetPlayerFlowComponent();
}

UAnimMontage* USSIMPlayerCombatComponent::GetAttackMontage()
{
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		// Air Attack Montage
		switch (PlayerAttackDirectionType)
		{
		case EPlayerAttackDirectionType::EPADT_Frontal:
			{
				if (PlayerAirFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirFrontalAttackMontages[FMath::RandHelper(PlayerAirFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirectionType::EPADT_Upward:
			{
				if (PlayerAirUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirUpwardAttackMontages[FMath::RandHelper(PlayerAirUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirectionType::EPADT_Downward:
			{
				if (PlayerPogoMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air DOWNWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerPogoMontages[FMath::RandHelper(PlayerPogoMontages.Num())];
				break;
			}
			
		default:
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s | Something went EXTREMELY wrong during attack animation search"), TEXT(__FUNCTION__));
				AttackMontage = nullptr;
				return nullptr;
			}
		}
	}
	else
	{
		// Ground Attack Montage
		switch (PlayerAttackDirectionType)
		{
		case EPlayerAttackDirectionType::EPADT_Frontal:
			{
				if (UpperBodyPlayerFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = UpperBodyPlayerFrontalAttackMontages[FMath::RandHelper(UpperBodyPlayerFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirectionType::EPADT_Upward:
			{
				if (PlayerUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerUpwardAttackMontages[FMath::RandHelper(PlayerUpwardAttackMontages.Num())];
				break;
			}
			
		default:
			{
				UE_LOG(LogSSIMValidations, Error, TEXT("%s | Something went EXTREMELY wrong during attack animation search"), TEXT(__FUNCTION__));
				AttackMontage = nullptr;
				return nullptr;
			}
		}
	}
	return AttackMontage;
}

void USSIMPlayerCombatComponent::OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	HitEnemies.AddUnique(OtherActor);
	
	HitRegistration(OtherActor);
}

// Internal
void USSIMPlayerCombatComponent::HitRegistration(AActor* OtherActor)
{
	DamageData.Instigator = SSIMPlayer;
	DamageData.Value = RegularAttackDamage;
	
	if (bShowAttackLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Error, TEXT("Hit Registration"));
	}
	
	if (!OtherActor->Implements<USSIMDamageableInterface>())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : Target does not implement USSIMDamageableInterface"), TEXT(__FUNCTION__));
		return;
	}
		
	if (bShowAttackLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Hit %s for %d damage"), TEXT(__FUNCTION__), *OtherActor->GetName(), RegularAttackDamage);
	}
		
	ISSIMDamageableInterface::Execute_ReceiveDamageInterface(OtherActor, DamageData);
	
	OnHitRegistrationDelegate.Broadcast(PlayerAttackDirectionType);
	
	if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Frontal && !bAttackKnockbackActive)
	{
		AttackKnockback();
	}
		
	if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward)
	{
		PogoInit();
	}
	
}

void USSIMPlayerCombatComponent::AttackKnockback()
{
	float AttackKnockbackVelocity;
	bool bShouldOverrideZ;
	
	if (!SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		AttackKnockbackType = EAttackKnockbackType::EAKT_Ground;
	}
	else
	{
		AttackKnockbackType = EAttackKnockbackType::EAKT_Air;
	}

	switch (AttackKnockbackType)
	{
	case EAttackKnockbackType::EAKT_Ground:
		{
			AttackKnockbackVelocity = GroundAttackKnockbackVelocity;
			bShouldOverrideZ = true;
			break;
		}
	case EAttackKnockbackType::EAKT_Air:
		{
			AttackKnockbackVelocity = AirAttackKnockbackVelocity;
			bShouldOverrideZ = false;
			break;
		}
	default:
		{
			AttackKnockbackVelocity = GroundAttackKnockbackVelocity;
			bShouldOverrideZ = false;
		}
	}
	
	bAttackKnockbackActive = true;
	OnAttackKnockbackStartedDelegate.Broadcast();
	
	SSIMPlayer->LaunchCharacter(SSIMPlayer->GetActorForwardVector() * -1.f * AttackKnockbackVelocity,true, bShouldOverrideZ);
	
	SSIMPlayer->GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	
	FTimerHandle KnockbackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		KnockbackTimerHandle,
		this, &USSIMPlayerCombatComponent::ResetAttackKnockbackState,
		AttackKnockbackDuration,
		false
		);
	
	if (bShowAttackKnockbackLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Perform Attack Knockback"), TEXT(__FUNCTION__));
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Player Forward Vector: %s"), TEXT(__FUNCTION__), *SSIMPlayer->GetActorForwardVector().ToString());
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Knockback Velocity: %f"), TEXT(__FUNCTION__), GroundAttackKnockbackVelocity);
	}
}

void USSIMPlayerCombatComponent::ResetAttackKnockbackState()
{
	SSIMPlayer->SetPlayerBrakingDecelerationWalkingToDefault();
	
	bAttackKnockbackActive = false;
	OnAttackKnockbackEndedDelegate.Broadcast();
}

#pragma region Pogo
void USSIMPlayerCombatComponent::PogoInit()
{
	const ACharacter* FirstHitEnemy = Cast<ACharacter>(HitEnemies[0]);
	
	if (bShowPogoLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Check Player = %f"), SSIMPlayer->GetActorLocation().Z - SSIMPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Check Enemy  = %f"), FirstHitEnemy->GetActorLocation().Z + FirstHitEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	}
	
	bPogoActive = true;
	OnPogoStartedDelegate.Broadcast();
	
	if (SSIMPlayer->GetActorLocation().Z - SSIMPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() 
		<	// Bottom of Player capsule below top of Enemy capsule
		FirstHitEnemy->GetActorLocation().Z + FirstHitEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
	{
		
		FVector AdjustedPlayerLocation  = SSIMPlayer->GetActorLocation();
		float AdjustedPlayerHeight = FirstHitEnemy->GetActorLocation().Z
								   + FirstHitEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
								   + SSIMPlayer->   GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		AdjustedPlayerLocation.Z = AdjustedPlayerHeight;
		
		if (bShowPogoLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Should Adjust Start Pogo Location"));
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Adjusted Player Height  = %f"), AdjustedPlayerHeight);
		}
		
		PogoAdjustHeightDelegate.BindUObject(this, &USSIMPlayerCombatComponent::AdjustPogoStartLocation, AdjustedPlayerLocation);
		
		SSIMPlayer->GetCharacterMovement()->GravityScale = 0.0f;
		SSIMPlayer->GetCharacterMovement()->StopMovementImmediately();
		SSIMPlayer->GetContactDamageCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
#if !UE_BUILD_SHIPPING
		
		if (bDrawPogoDebug)
		{
			UKismetSystemLibrary::DrawDebugPoint(GetWorld(),
											 	SSIMPlayer->GetActorLocation(),
											 	32.f,
											 	PogoAdjustmentStartPointColor,
											 	bDrawPogoDebugDuration);
			
			UKismetSystemLibrary::DrawDebugPoint(GetWorld(),
												 AdjustedPlayerLocation,
												 25.f,
												 PogoAdjustmentEndPointColor,
												 bDrawPogoDebugDuration);
			
			UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
										SSIMPlayer->GetActorLocation(), 
										 AdjustedPlayerLocation, 
									   10.f, 
												 PogoAdjustmentDirectionArrowColor, 
												 bDrawPogoDebugDuration, 
									   5.f);
		}
#endif
		
		GetWorld()->GetTimerManager().SetTimer(
			PogoAdjustHeightHandle,
			PogoAdjustHeightDelegate,
			PogoInterpolationStepTime,
			true
		);
	}
	else
	{
		PogoStart();
	}
}

void USSIMPlayerCombatComponent::AdjustPogoStartLocation(FVector AdjustedPlayerLocation)
{
	SSIMPlayer->SetActorLocation(
					FMath::VInterpConstantTo(
						SSIMPlayer->GetActorLocation(),
						AdjustedPlayerLocation,
						PogoInterpolationStepTime,
						PogoAdjustmentInterpSpeed));
	
	if (SSIMPlayer->GetActorLocation().Equals(AdjustedPlayerLocation, 0.5f))
	{
		GetWorld()->GetTimerManager().ClearTimer(PogoAdjustHeightHandle);
		PogoStart();
	}
}

void USSIMPlayerCombatComponent::PogoStart()
{	
	SSIMPlayer->GetCharacterMovement()->GravityScale = PogoTemporaryGravityScale;
	
	GetWorld()->GetTimerManager().SetTimer(
		EndPogoTimerHandle,
		EndPogoTimerDelegate,
		PogoStateDuration,
		false
		);
	
	FVector CachedPlayerForwardVector = SSIMPlayer->GetActorForwardVector();
	float ModifiedPogoAngle = PogoAngle;
	
	if (FMath::IsNearlyEqual(CachedPlayerForwardVector.Y, -FVector::RightVector.Y, 0.5))
	{
		ModifiedPogoAngle *= -1.f;
	}
	
	FVector ReboundDirection = CachedPlayerForwardVector.RotateAngleAxis(ModifiedPogoAngle, FVector::XAxisVector);
	
	SSIMPlayer->LaunchCharacter(ReboundDirection * PogoVelocity, true, true);
	
	SSIMPlayer->GetContactDamageCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
#if !UE_BUILD_SHIPPING
	if (bDrawPogoDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
								SSIMPlayer->GetActorLocation(), 
								 SSIMPlayer->GetActorLocation() + (ReboundDirection * 250.f), 
							   25.f, 
										 PogoReboundDirectionArrowColor, 
										 bDrawPogoDebugDuration, 
							   5.f);
	}
	
#endif
}

void USSIMPlayerCombatComponent::EndPogo()
{
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
	bPogoActive = false;
	OnPogoEndedDelegate.Broadcast();
}
#pragma endregion Pogo

#pragma region Handlers
void USSIMPlayerCombatComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	EndAttack(); // interrupt attack to avoid stuck in attack in case of mutual attack
}

void USSIMPlayerCombatComponent::OnDashStartedHandler()
{
	EndAttack(); // interrupt attack so to avoid stuck in attack if dash interrupts attack
}

void USSIMPlayerCombatComponent::PogoAnimationCallback(UAnimMontage* PogoMontage, bool Interrupted) const
{
	OnPogoAnimationEndedDelegate.Broadcast(Interrupted);
}
#pragma endregion Handlers

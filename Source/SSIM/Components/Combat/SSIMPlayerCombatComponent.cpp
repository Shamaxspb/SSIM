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


void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDamageReceivedHandler);
	PlayerFlowComponent->OnDashStartedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDashStartedHandler);
	
	EndPogoTimerDelegate.BindUObject(this, &USSIMPlayerCombatComponent::EndPogo);
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
				if (PlayerFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerFrontalAttackMontages[FMath::RandHelper(PlayerFrontalAttackMontages.Num())];			
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
	
	if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward)
	{
		PogoInit();
		return;
	}
	
	DealDamageToEnemy();
}

void USSIMPlayerCombatComponent::DealDamageToEnemy()
{
	if (HitEnemies.IsEmpty())
	{
		if (bShowAttackLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Hit nothing"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	DamageData.Instigator = SSIMPlayer;
	DamageData.Value = RegularAttackDamage;
	
	for (auto Element : HitEnemies)
	{
		if (!Element->Implements<USSIMDamageableInterface>())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s : Target does not implement USSIMDamageableInterface"), TEXT(__FUNCTION__));
			return;
		}
		
		if (bShowAttackLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Hit %s"), TEXT(__FUNCTION__), *Element->GetName());
		}
		
		if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward
			&&
			SSIMPlayer->GetPlayerPogoState())
		{
			// If Pogo is active and hit another target - restart EndPogo timer
		GetWorld()->GetTimerManager().SetTimer(
			EndPogoTimerHandle,
			EndPogoTimerDelegate,
			PogoStateDuration,
			false
			);
		}
		
		ISSIMDamageableInterface::Execute_ReceiveDamageInterface(Element, DamageData);
	}
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
	
	if (bShowPogoLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Adjustment: Current Location - %s"), *SSIMPlayer->GetActorLocation().ToString());
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Adjustment: Target Location  - %s"), *AdjustedPlayerLocation.ToString());
	}
	
	
	if (SSIMPlayer->GetActorLocation().Equals(AdjustedPlayerLocation, 0.5f))
	{
		GetWorld()->GetTimerManager().ClearTimer(PogoAdjustHeightHandle);
		PogoStart();
	}
}

void USSIMPlayerCombatComponent::PogoStart()
{	
	DealDamageToEnemy();
	
	SSIMPlayer->GetCharacterMovement()->GravityScale = PogoTemporaryGravityScale;
	
	// EndPogoTimerDelegate.BindUObject(this, &USSIMPlayerCombatComponent::EndPogo);
	
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

void USSIMPlayerCombatComponent::EndPogo() const
{
	/*if (SSIMPlayer->GetPlayerAttackingState())
	{
		if (bShowPogoLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("%s | EndPogo CANCELED, Player is still attacking/started next attack"), 
				TEXT(__FUNCTION__), *SSIMPlayer->GetActorLocation().ToString());
		}
		return;
	}*/
	
	SSIMPlayer->SetPlayerGravityScaleToDefault();
	
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

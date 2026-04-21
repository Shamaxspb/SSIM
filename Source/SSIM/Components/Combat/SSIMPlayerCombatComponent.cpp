// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMPlayerStatsComponent>();
	StatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDamageReceivedHandler);
}

// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	Super::StartAttack();
	
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack Direction: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PlayerAttackDirection));
}

void USSIMPlayerCombatComponent::StartAttackTrace()
{	
	switch (PlayerAttackDirection)
	{
		case EPlayerAttackDirection::EPAD_Frontal:
		{
			CurrentAttackCollision = SSIMPlayer->GetFrontalAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Upward:
		{
			CurrentAttackCollision = SSIMPlayer->GetUpperAttackCollision();
			break;
		}
		
		case EPlayerAttackDirection::EPAD_Downward:
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
}

UAnimMontage* USSIMPlayerCombatComponent::GetAttackMontage()
{
	if (SSIMPlayer->GetCharacterMovement()->IsFalling())
	{
		// Air Attack Montage
		switch (PlayerAttackDirection)
		{
		case EPlayerAttackDirection::EPAD_Frontal:
			{
				if (PlayerAirFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirFrontalAttackMontages[FMath::RandHelper(PlayerAirFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Upward:
			{
				if (PlayerAirUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No AIR UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirUpwardAttackMontages[FMath::RandHelper(PlayerAirUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Downward:
			{
				if (PlayerAirDownwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No Air DOWNWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerAirDownwardAttackMontages[FMath::RandHelper(PlayerAirDownwardAttackMontages.Num())];
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
		switch (PlayerAttackDirection)
		{
		case EPlayerAttackDirection::EPAD_Frontal:
			{
				if (PlayerFrontalAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No FRONTAL Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerFrontalAttackMontages[FMath::RandHelper(PlayerFrontalAttackMontages.Num())];			
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Upward:
			{
				if (PlayerUpwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No UPWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerUpwardAttackMontages[FMath::RandHelper(PlayerUpwardAttackMontages.Num())];
				break;
			}
	
		case EPlayerAttackDirection::EPAD_Downward:
			{
				if (PlayerDownwardAttackMontages.IsEmpty())
				{
					UE_LOG(LogSSIMValidations, Error, TEXT("%s | No DOWNWARD Attack Montages found"), TEXT(__FUNCTION__));
					return nullptr;
				}
				AttackMontage = PlayerDownwardAttackMontages[FMath::RandHelper(PlayerDownwardAttackMontages.Num())];
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
	HitEnemies.Add(OtherActor);
	UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Hit Enemy : %s"), TEXT(__FUNCTION__), *OtherActor->GetName());
	
	DealDamageToEnemy();
}

void USSIMPlayerCombatComponent::DealDamageToEnemy()
{
	if (HitEnemies.IsEmpty())
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Hit nothing"), TEXT(__FUNCTION__));
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
		
		ISSIMDamageableInterface::Execute_ReceiveDamageInterface(Element, DamageData);
		LaunchTargetOnHit(Element);
	}
}

void USSIMPlayerCombatComponent::LaunchTargetOnHit(AActor* InActor) const
{
	ACharacter* Target = Cast<ACharacter>(InActor);
	
	Target->LaunchCharacter(CalculateOnHitLaunchVelocity(Target), false, false);
}

FVector USSIMPlayerCombatComponent::CalculateOnHitLaunchVelocity(const AActor* InActor) const
{
	FVector PlayerLocation = SSIMPlayer->GetActorLocation();
	FVector EnemyLocation = InActor->GetActorLocation();
	
	// Determine if Enemy is to the right or to the left
	bool bEnemyToTheRight = EnemyLocation.Y > PlayerLocation.Y;
	
	// Get unit vector from Enemy to Player and Negate that vector
	FVector ReboundDirection = UKismetMathLibrary::NegateVector(UKismetMathLibrary::GetDirectionUnitVector(EnemyLocation, PlayerLocation));
	
	// Add rotation to that vector (around X axis)
	FVector RotatedDirection = ReboundDirection.RotateAngleAxis(bEnemyToTheRight ? ReboundAngle : -ReboundAngle, FVector::ForwardVector);
	
	// Multiply by coef for launch
	FVector ReboundVelocity = RotatedDirection * ReboundVelocityCoef;
	
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Enemy Is to the: %s"), (bEnemyToTheRight ? TEXT("Right") : TEXT("Left")));
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Rebound Direction: %s"), *ReboundDirection.ToString());
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Rotated Direction: %s"), *RotatedDirection.ToString());
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Rebound Velocity: %s"),  *ReboundVelocity.ToString());
	
	#if !UE_BUILD_SHIPPING
	if (bReboundShowDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
									EnemyLocation, 
									 EnemyLocation + (RotatedDirection * 250.f), 
								   25.f, 
											 ReboundDirectionArrowColor, 
									 3.f, 
								   5.f);
	}
	#endif !UE_BUILD_SHIPPING
	
	return ReboundVelocity;
	// This is so messy because it was hard to understand how the fck should I implement this
	// would be nice to clean this up later
}

void USSIMPlayerCombatComponent::OnDamageReceivedHandler(const FDamageData InDamageData)
{
	EndAttack(); // interrupt attack to avoid stuck in attack in case of mutual attack
}


//DEBUG
void USSIMPlayerCombatComponent::SwitchAttackCollision_DEBUG() const
{
	if (!IsValid(CurrentAttackCollision))
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("No current attack collision"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, 
										TEXT("No current attack collision"), false, FVector2D(1.2f, 1.2f));
		return;
	}
	
	if (CurrentAttackCollision->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
	{
		CurrentAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CurrentAttackCollision->SetHiddenInGame(true);
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Current attack collision: NoCollision"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Red, 
										TEXT("Current attack collision: NoCollision"), false, FVector2D(1.2f, 1.2f));
	}
	else if (CurrentAttackCollision->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		CurrentAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CurrentAttackCollision->SetHiddenInGame(false);
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("Current attack collision: QueryOnly"));
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Emerald, 
										TEXT("Current attack collision: QueryOnly"), false, FVector2D(1.2f, 1.2f));
	}
}

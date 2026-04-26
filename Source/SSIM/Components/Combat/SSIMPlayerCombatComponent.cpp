// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerCombatComponent.h"

#include "SSIM/SSIM.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"
#include "SSIM/Characters/Player/SSIMPlayer.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerFlowComponent.h"


void USSIMPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetReferences();
	
	PlayerStatsComponent->OnDamageReceivedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDamageReceivedHandler);
	PlayerFlowComponent->OnDashStartedDelegate.AddDynamic(this, &USSIMPlayerCombatComponent::OnDashStartedHandler);
}

// My Functions
void USSIMPlayerCombatComponent::StartAttack()
{
	Super::StartAttack();
	if (bShowLogs)
    {
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Attack Direction: %s"), TEXT(__FUNCTION__), *UEnum::GetValueAsString(PlayerAttackDirectionType));
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
	if (bShowLogs)
	{
		UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s | Hit Enemy : %s"), TEXT(__FUNCTION__), *OtherActor->GetName());
	}
	
	DealDamageToEnemy();
}

void USSIMPlayerCombatComponent::DealDamageToEnemy()
{
	if (HitEnemies.IsEmpty())
	{
		if (bShowLogs)
		{
			UE_LOG(LogSSIMGameplayMessages, Log, TEXT("%s : Hit nothing"), TEXT(__FUNCTION__));
		}
		return;
	}
	
	DamageData.Instigator = SSIMPlayer;
	DamageData.Value = RegularAttackDamage;
	
	if (PlayerAttackDirectionType == EPlayerAttackDirectionType::EPADT_Downward)
	{
		PogoInit();
	}
	
	for (auto Element : HitEnemies)
	{
		if (!Element->Implements<USSIMDamageableInterface>())
		{
			UE_LOG(LogSSIMValidations, Error, TEXT("%s : Target does not implement USSIMDamageableInterface"), TEXT(__FUNCTION__));
			return;
		}
		
		ISSIMDamageableInterface::Execute_ReceiveDamageInterface(Element, DamageData);
	}
}

void USSIMPlayerCombatComponent::PogoInit()
{
	const ACharacter* FirstHitEnemy = Cast<ACharacter>(HitEnemies[0]);
	
	PogoAdjustLocationDelegate.BindUObject(this, &USSIMPlayerCombatComponent::PogoAdjustLocation, FirstHitEnemy);
	
	if (SSIMPlayer->GetActorLocation().Z < FirstHitEnemy->GetActorLocation().Z + FirstHitEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
	{
		GetWorld()->GetTimerManager().SetTimer(
			PogoAdjustLocationHandle,
			PogoAdjustLocationDelegate,
			0.01f,
			true
		);
	}
	else
	{
		PogoStart();
	}
}

void USSIMPlayerCombatComponent::PogoAdjustLocation(const ACharacter* InFirstHitEnemy)
{
	FVector AdjustedPlayerLocation = SSIMPlayer->GetActorLocation();
	
	AdjustedPlayerLocation.Y = InFirstHitEnemy->GetActorLocation().Y;
	
	// Sum Player's and Enemy's capsule HalfHeights and adding a little bit 
	AdjustedPlayerLocation.Z = InFirstHitEnemy->GetActorLocation().Z + 
							  (InFirstHitEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() +
							  + SSIMPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
							  * 1.05;
	
	constexpr float StepTime = 0.05f;
	
	SSIMPlayer->SetActorLocation(
		FMath::VInterpConstantTo(
			SSIMPlayer->GetActorLocation(), 
			AdjustedPlayerLocation, 
			StepTime,
			interpSpeed)
		);
	
	if (SSIMPlayer->GetActorLocation().Equals(AdjustedPlayerLocation, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(PogoAdjustLocationHandle);
		PogoStart();
	}
}

void USSIMPlayerCombatComponent::PogoStart()
{	
	SSIMPlayer->GetCharacterMovement()->GravityScale = PogoTemporaryGravityScale;
	
	FTimerHandle GravityResetTimerHandle;
	FTimerDelegate GravityResetTimerDelegate; 
	GravityResetTimerDelegate.BindUObject(this, &USSIMPlayerCombatComponent::EndPogo);
	
	GetWorld()->GetTimerManager().SetTimer(
		GravityResetTimerHandle,
		GravityResetTimerDelegate,
		PogoTemporaryGravityDuration,
		false
		);
	
	FVector CachedPlayerForwardVector = SSIMPlayer->GetActorForwardVector();
	float ModifiedPogoAngle = PogoAngle;
	
	if (FMath::IsNearlyEqual(CachedPlayerForwardVector.Y, -FVector::RightVector.Y, 0.5))
	{
		ModifiedPogoAngle *= -1.f;
	}
	
	FVector ReboundDirection = CachedPlayerForwardVector.RotateAngleAxis(ModifiedPogoAngle, FVector::XAxisVector);
	
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Player Forward Vector: %s"), *SSIMPlayer->GetActorForwardVector().ToString());
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Cached Player Forward Vector: %s"), *CachedPlayerForwardVector.ToString());
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Pogo Angle: %f"), PogoAngle);
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Modified PogoAngle: %f"), ModifiedPogoAngle);
	
	SSIMPlayer->LaunchCharacter(ReboundDirection * PogoVelocity, true, true);
	
	OnPogoStartedDelegate.Broadcast();
	
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("ReboundDirection: %s"), *ReboundDirection.ToString());
	UE_LOG(LogSSIMGameplayMessages, Warning, TEXT("Launch Velocity: %s"), *FVector(ReboundDirection * PogoVelocity).ToString());
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), 
								SSIMPlayer->GetActorLocation(), 
								 SSIMPlayer->GetActorLocation() + (ReboundDirection * 250.f), 
							   25.f, 
										 FLinearColor::Yellow, 
										 5.f, 
							   5.f);
}

void USSIMPlayerCombatComponent::EndPogo() const
{
	SSIMPlayer->GetCharacterMovement()->GravityScale = SSIM_DEFAULT_PLAYER_GRAVITY_SCALE;
	OnPogoEndedDelegate.Broadcast();
}

void USSIMPlayerCombatComponent::OnDamageReceivedHandler(const FDamageData& InDamageData)
{
	EndAttack(); // interrupt attack to avoid stuck in attack in case of mutual attack
}

void USSIMPlayerCombatComponent::OnDashStartedHandler()
{
	EndAttack(); // interrupt attack so to avoid stuck in attack if dash interrupts attack
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Components/Combat/SSIMEnemyCombatComponent.h"
#include "SSIM/Components/Stats/SSIMEnemyStatsComponent.h"
#include "SSIM/Components/DamageReaction/SSIMEnemyDamageReactionComponent.h"
#include "SSIM/Core/SSIMGameplayTags.h"
#include "SSIM/Core/Types/SSIMEnemyDataAsset.h"
#include "SSIM/Core/Types/SSIMPlayerTypes.h"

// Overriden Functions
ASSIMBaseEnemy::ASSIMBaseEnemy()
{
	GetCapsuleComponent()->SetCollisionProfileName("Enemy", true);
	GetMesh()->SetCollisionProfileName("Enemy", true);
	
	EnemyCombatComponent		  = CreateDefaultSubobject<USSIMEnemyCombatComponent>(TEXT("CombatComponent"));
	EnemyStatsComponent			  = CreateDefaultSubobject<USSIMEnemyStatsComponent>(TEXT("StatsComponent"));
	EnemyDamageReactionComponent  = CreateDefaultSubobject<USSIMEnemyDamageReactionComponent>(TEXT("DamageReactionComponent"));
	
	SetupAttackCollision();
}

void ASSIMBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyFacingDirection = FVector::DotProduct(GetActorForwardVector(), FVector::RightVector) > 0.f ?
												EFacingDirection::EPD_Right : EFacingDirection::EPD_Left;
}

void ASSIMBaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateFacingDirection();
}

void ASSIMBaseEnemy::InitializeAttributes()
{
	if (!IsValid(EnemyData))
	{
		UE_LOG(LogSSIMEnemyInitialization, Error, TEXT("%s | Enemy Data Asset is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	const FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		EnemyData->InitializationEffect,
		1.0f,
		EffectContext);
	
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s | SpecHandle is not valid"), TEXT(__FUNCTION__));
		return;
	}
	
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_MaxHealth, EnemyData->InitialAttributes.MaxHealth);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Attribute_Health, EnemyData->InitialAttributes.Health);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	/*UE_LOG(LogSSIMEnemyInitialization, Log, TEXT("%s | %s : MaxHealth - %f, Health - %f"), 
		TEXT(__FUNCTION__),
		*GetName(),
		EnemyData->InitialAttributes.MaxHealth,
		EnemyData->InitialAttributes.Health);*/
}

void ASSIMBaseEnemy::SetupAttackCollision()
{
	AttackBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackBoxCollision->SetupAttachment(GetRootComponent());
	
	AttackBoxCollision->SetGenerateOverlapEvents(true);
	AttackBoxCollision->SetCollisionProfileName("MeleeAttack", true);
	AttackBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASSIMBaseEnemy::SetEnemyFacingDirection(EFacingDirection InEnemyFacingDirection)
{
	EnemyFacingDirection = InEnemyFacingDirection;
	FRotator EnemyRotation = GetActorRotation();
	switch (InEnemyFacingDirection)
	{
	case EFacingDirection::EPD_Right:
		{
			EnemyRotation.Yaw = 90.f;
			break;
		}
	case EFacingDirection::EPD_Left:
		{
			EnemyRotation.Yaw = -90.f;
			break;
		}
	}
	SetActorRotation(EnemyRotation);
}

void ASSIMBaseEnemy::UpdateFacingDirection()
{
	if (GetVelocity().Y > 0.f)
	{
		EnemyFacingDirection = EFacingDirection::EPD_Right;
	}
	else if (GetVelocity().Y < 0.f)
	{
		EnemyFacingDirection = EFacingDirection::EPD_Left;
	}
}

#pragma region Interfaces
void ASSIMBaseEnemy::StartAttackInterface_Implementation() const
{
	EnemyCombatComponent->StartAttack();
}

void ASSIMBaseEnemy::EndAttackInterface_Implementation() const
{
	EnemyCombatComponent->EndAttack();
}

void ASSIMBaseEnemy::StartAttackTraceInterface_Implementation() const
{
	EnemyCombatComponent->StartAttackTrace();
}

void ASSIMBaseEnemy::EndAttackTraceInterface_Implementation() const
{
	EnemyCombatComponent->EndAttackTrace();
}

void ASSIMBaseEnemy::ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const
{
	EnemyStatsComponent->ReduceHealth(InDamageData);
}

void ASSIMBaseEnemy::ReceivePlayerAttackDirectionType_Implementation(
	EPlayerAttackDirectionType InPlayerAttackDirectionType) const
{
	EnemyDamageReactionComponent->ReceivePlayerAttackDirectionType(InPlayerAttackDirectionType);
}
#pragma endregion Interfaces

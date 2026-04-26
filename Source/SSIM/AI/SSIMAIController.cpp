// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMAIController.h"

#include "SSIM/Core/Helpers/SSIMBlackboardHelper.h"
#include "SSIM/SSIM.h"
#include "SSIM/Characters/Enemies/SSIMBaseEnemy.h"
#include "SSIM/Components/DamageReaction/SSIMEnemyDamageReactionComponent.h"


ASSIMAIController::ASSIMAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASSIMAIController::BeginPlay()
{
	Super::BeginPlay();
	
	SetBlackboardStaggeredDuration();
}

void ASSIMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!BaseBehaviorTree)
	{
		if (GetWorld()->IsGameWorld())
		{
			UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("%s | Base Behavior Tree is not valid"), TEXT(__FUNCTION__));
		}
		return;
	}
	RunBehaviorTree(BaseBehaviorTree);
	
	SetReferences();
	
	UE_LOG(LogTemp, Warning, TEXT("Binding to component: %s"), *GetNameSafe(EnemyDamageReactionComponent));
	
	EnemyDamageReactionComponent->OnStartStaggerDelegate.AddDynamic(this, &ASSIMAIController::OnStartStaggerHandler);
	EnemyDamageReactionComponent->OnEndStaggerDelegate.AddDynamic(this, &ASSIMAIController::OnEndStaggerHandler);
}


void ASSIMAIController::SetReferences()
{
	BaseEnemy = Cast<ASSIMBaseEnemy>(GetPawn());
	EnemyDamageReactionComponent = BaseEnemy->GetEnemyDamageReactionComponent();
}

void ASSIMAIController::OnStartStaggerHandler()
{
	SetBlackboardEnemyState(EEnemyState::EES_Staggered);
}

void ASSIMAIController::OnEndStaggerHandler()
{
	SetBlackboardEnemyState(EEnemyState::EES_Combat);
}

void ASSIMAIController::SetBlackboardEnemyState(EEnemyState InNewState) const
{
	uint8 NewState = static_cast<uint8>(InNewState);
	USSIMBlackboardHelper::SetEnumSafe(Blackboard, TEXT("EEnemyState"), NewState);
}

void ASSIMAIController::SetBlackboardStaggeredDuration() const
{
	USSIMBlackboardHelper::SetFloatSafe(Blackboard, TEXT("StaggerDuration"), EnemyDamageReactionComponent->StaggerDuration);
}

void ASSIMAIController::DebugComponents()
{
	if (!BaseEnemy)
	{
		UE_LOG(LogTemp, Error, TEXT("BaseEnemy is NULL"));
		return;
	}

	TArray<UActorComponent*> Components;
	BaseEnemy->GetComponents(Components);

	UE_LOG(LogSSIMEnemyInitialization, Warning, TEXT("=== COMPONENT LIST FOR %s ==="), *BaseEnemy->GetName());

	for (UActorComponent* Comp : Components)
	{
		UE_LOG(LogSSIMEnemyInitialization, Warning, TEXT("Component: %s | %s | Outer: %s"),
			*Comp->GetName(),
			*Comp->GetClass()->GetName(),
			*GetNameSafe(Comp->GetOuter()));
	}
}



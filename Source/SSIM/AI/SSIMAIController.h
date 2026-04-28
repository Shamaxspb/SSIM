// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "SSIMAIController.generated.h"

struct FDamageData;
enum class EEnemyState : uint8;
class ASSIMBaseEnemy;
class USSIMEnemyStatsComponent;
class USSIMEnemyDamageReactionComponent;
class UBehaviorTree;


UCLASS()
class SSIM_API ASSIMAIController : public AAIController
{
	GENERATED_BODY()
	
// Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|AI")
	TObjectPtr<UBehaviorTree> BaseBehaviorTree;
	
	UPROPERTY()
	TObjectPtr<ASSIMBaseEnemy> BaseEnemy;
	UPROPERTY()
	TObjectPtr<USSIMEnemyDamageReactionComponent> EnemyDamageReactionComponent;

// Overriden Functions
public:
	ASSIMAIController();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	
// My Functions
private:
	void SetReferences();
	
	UFUNCTION()
	void OnStartStaggerHandler();
	UFUNCTION()
	void OnEndStaggerHandler();
	
	void SetBlackboardEnemyState(EEnemyState InNewState) const;
	void SetBlackboardStaggeredDuration() const;
	
	// DEBUG
	void DebugComponents();
	
	UFUNCTION(BlueprintCallable)
	void PrintEnemyStateValue() const;
};

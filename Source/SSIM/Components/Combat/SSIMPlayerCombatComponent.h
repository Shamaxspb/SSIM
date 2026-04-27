// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerCombatComponent.generated.h"

class USSIMPlayerFlowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPogoStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPogoEndedSignature);

class ASSIMPlayer;
class UBoxComponent;
class USSIMPlayerStatsComponent;

UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerCombatComponent : public USSIMBaseCombatComponent
{
	GENERATED_BODY()

// Variables
public:
	// Delegates
	FOnPogoStartedSignature OnPogoStartedDelegate;
	FOnPogoEndedSignature OnPogoEndedDelegate;
	
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack")
	float RegularAttackDamage;
	
#pragma endregion Stats	
	
#pragma region Montages
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirDownwardAttackMontages;

#pragma endregion Montages
	
#pragma region Metadata
	
public:
	EPlayerAttackDirectionType PlayerAttackDirectionType;

#pragma endregion Metadata
	
private:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
	UPROPERTY()
	TObjectPtr<USSIMPlayerStatsComponent> PlayerStatsComponent;
	
	UPROPERTY()
	TObjectPtr<USSIMPlayerFlowComponent> PlayerFlowComponent;

#pragma region Pogo
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Pogo", meta = (AllowPrivateAccess = true))
	float PogoAngle = 50.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Pogo")
	float PogoVelocity = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Pogo")
	float PogoTemporaryGravityScale = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Pogo")
	float PogoStateDuration = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Pogo|Settings")
	float PogoAdjustmentInterpSpeed = 1500.f;
	
#pragma region Pogo Debug
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Pogo")
	bool bShowPogoDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug", EditConditionHides,
																	  DisplayAfter = "bShowPogoDebug"))
	bool bShowPogoLogs;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug", EditConditionHides,
																 	  DisplayAfter = "bShowPogoLogs"))
	bool bDrawPogoDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug && bDrawPogoDebug", EditConditionHides,
																 	  DisplayAfter = "bDrawPogoDebug"))
	float bDrawPogoDebugDuration = 5.f;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug && bDrawPogoDebug", EditConditionHides,
															 	  DisplayAfter = "bDrawPogoDebug"))
	FLinearColor PogoAdjustmentStartPointColor = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug && bDrawPogoDebug", EditConditionHides,
															 	  DisplayAfter = "bDrawPogoDebug"))
	FLinearColor PogoAdjustmentEndPointColor = FLinearColor::Green;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug && bDrawPogoDebug", EditConditionHides,
															 	  DisplayAfter = "bDrawPogoDebug"))
	FLinearColor PogoAdjustmentDirectionArrowColor = FLinearColor::Yellow;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bShowPogoDebug && bDrawPogoDebug", EditConditionHides,
																  DisplayAfter = "bDrawPogoDebug"))
	FLinearColor PogoReboundDirectionArrowColor = FLinearColor::Blue;
#pragma endregion Pogo Debug
	
	
private:
	FTimerHandle PogoAdjustHeightHandle;
	FTimerDelegate PogoAdjustHeightDelegate;
	float PogoInterpolationStepTime = 0.002f;
	
#pragma endregion Pogo


// Overriden Functions
protected:
	virtual void BeginPlay() override;

// My Functions
public:
	virtual void StartAttack() override;
	virtual void StartAttackTrace() override;

protected:
	virtual void SetReferences() override;
	virtual UAnimMontage* GetAttackMontage() override;
	
	virtual void OnAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
											   AActor* OtherActor,
											   UPrimitiveComponent* OtherComp,
											   int32 OtherBodyIndex,
											   bool bFromSweep,
											   const FHitResult& SweepResult) override;
	
private:
	void DealDamageToEnemy();
	
	void PogoInit();
	void AdjustPogoStartLocation(FVector AdjustedPlayerLocation);
	void PogoStart();
	void EndPogo() const;
	
	void SetMeshTransformForPogo(bool bPogoStart) const;
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);
	

	UFUNCTION()
	void OnDashStartedHandler();

};

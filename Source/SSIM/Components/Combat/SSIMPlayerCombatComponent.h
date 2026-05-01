// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerCombatComponent.generated.h"

class USSIMPlayerFlowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitRegistrationSignature, EPlayerAttackDirectionType, AttackDirectionType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPogoStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPogoEndedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPogoAnimationStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPogoAnimationEndedSignature, bool, bInterrupted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackKnockbackStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackKnockbackEndedSignature);

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
	FOnHitRegistrationSignature		   OnHitRegistrationDelegate;
	FOnPogoStartedSignature			   OnPogoStartedDelegate;
	FOnPogoEndedSignature			   OnPogoEndedDelegate;
	FOnPogoAnimationStartedSignature   OnPogoAnimationStartedDelegate;
	FOnPogoAnimationEndedSignature	   OnPogoAnimationEndedDelegate;
	FOnAttackKnockbackStartedSignature OnAttackKnockbackStartedDelegate;
	FOnAttackKnockbackEndedSignature   OnAttackKnockbackEndedDelegate;
	
	bool bAttackKnockbackActive = false;
	bool bPogoActive 			= false;
	
	
#pragma region Stats
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack")
	int32 RegularAttackDamage;
	
#pragma endregion Stats	
	
#pragma region Montages
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|FrontalAttack")
	TArray<TObjectPtr<UAnimMontage>> UpperBodyPlayerFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|FrontalAttack")
	TArray<TObjectPtr<UAnimMontage>> FullBodyPlayerFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|FrontalAttack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirFrontalAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|UpwardAttack")
	TArray<TObjectPtr<UAnimMontage>> PlayerUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|UpwardAttack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirUpwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|PogoAttack")
	TArray<TObjectPtr<UAnimMontage>> PlayerPogoMontages;

#pragma endregion Montages
	
#pragma region Metadata
	
public:
	EPlayerAttackDirectionType PlayerAttackDirectionType;
	EAttackKnockbackType AttackKnockbackType;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Pogo")
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
	bool bShowPogoLogs;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Pogo")
	bool bDrawPogoDebug;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bDrawPogoDebug", EditConditionHides))
	float bDrawPogoDebugDuration = 5.f;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bDrawPogoDebug", EditConditionHides))
	FLinearColor PogoAdjustmentStartPointColor = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bDrawPogoDebug", EditConditionHides))
	FLinearColor PogoAdjustmentEndPointColor = FLinearColor::Green;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bDrawPogoDebug", EditConditionHides))
	FLinearColor PogoAdjustmentDirectionArrowColor = FLinearColor::Yellow;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|DEBUG|Pogo", meta = (EditCondition = "bDrawPogoDebug", EditConditionHides))
	FLinearColor PogoReboundDirectionArrowColor = FLinearColor::Blue;
#pragma endregion Pogo Debug
	
	
private:
	FTimerHandle EndPogoTimerHandle;
	FTimerDelegate EndPogoTimerDelegate;
	
	FTimerHandle PogoAdjustHeightHandle;
	FTimerDelegate PogoAdjustHeightDelegate;
	float PogoInterpolationStepTime = 0.002f;
	
#pragma endregion Pogo

	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Attack")
	float GroundAttackKnockbackVelocity;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Attack")
	float AirAttackKnockbackVelocity;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Combat|Attack")
	float AttackKnockbackDuration = 0.1f;

	// Debug
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Debug|Attack")
	bool bShowAttackKnockbackLogs;
	
// Overriden Functions
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	USSIMPlayerCombatComponent();
	
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
	void HitRegistration();
	
	void AttackKnockback();
	void ResetAttackKnockbackState();
	
	void PogoInit();
	void AdjustPogoStartLocation(FVector AdjustedPlayerLocation);
	void PogoStart();
	void EndPogo();
	
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);

	UFUNCTION()
	void OnDashStartedHandler();
	
	UFUNCTION()
	void PogoAnimationCallback(UAnimMontage* PogoMontage, bool Interrupted) const;

};

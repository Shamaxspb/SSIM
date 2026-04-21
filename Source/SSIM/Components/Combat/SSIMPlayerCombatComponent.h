// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIMBaseCombatComponent.h"
#include "SSIM/Core/Types/SSIMCombatDataTypes.h"

#include "SSIMPlayerCombatComponent.generated.h"

class ASSIMPlayer;
class UBoxComponent;


UCLASS(Blueprintable, ClassGroup=(PlayerComponents))
class SSIM_API USSIMPlayerCombatComponent : public USSIMBaseCombatComponent
{
	GENERATED_BODY()

// Variables
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
	TArray<TObjectPtr<UAnimMontage>> PlayerDownwardAttackMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations|Attack")
	TArray<TObjectPtr<UAnimMontage>> PlayerAirDownwardAttackMontages;

#pragma endregion Montages
	
#pragma region Metadata
	
public:
	EPlayerAttackDirection PlayerAttackDirection;
	
#pragma endregion Metadata
	
protected:
	UPROPERTY()
	TObjectPtr<ASSIMPlayer> SSIMPlayer;
	
#pragma region Rebound
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack|Rebound")
	float ReboundAngle = 45.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Combat|Attack|Rebound")
	float ReboundVelocityCoef = 700.f;
	
	FRotator ReboundRotator = FRotator(ReboundAngle, 0.f, 0.f);
	
	// Debug
	
	UPROPERTY(EditAnywhere, Category = "SSIM|Combat|Attack|Rebound|DEBUG")
	bool bReboundShowDebug;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|Combat|Attack|Rebound|DEBUG", meta = (EditCondition = "bReboundShowDebug", EditConditionHides))
	bool bDrawReboundDirectionArrow;
	
	UPROPERTY(EditAnywhere, Category = "SSIM|Combat|Attack|Rebound|DEBUG", meta = (EditCondition = "bReboundShowDebug && bDrawReboundDirectionArrow", EditConditionHides))
	FLinearColor ReboundDirectionArrowColor = FLinearColor(1.f, 0.287f, 0.017f, 1.f);
	
#pragma endregion Rebound
	
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
	void LaunchTargetOnHit(AActor* InActor) const; 
	FVector CalculateOnHitLaunchVelocity(const AActor* InActor) const;
	
// DEBUG
	UFUNCTION(BlueprintCallable, Category = "SSIM|DEBUG")
	void SwitchAttackCollision_DEBUG() const;
};

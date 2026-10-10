// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"

#include "SSIMBaseEnemy.generated.h"

class USSIMEnemyDataAsset;
enum class EFacingDirection : uint8;
class USSIMEnemyDamageReactionComponent;
class USSIMEnemyCombatComponent;
class USSIMEnemyStatsComponent;
class UBoxComponent;

UCLASS(Abstract)
class SSIM_API ASSIMBaseEnemy : public ASSIMBaseCharacter, public ISSIMCombatInterface,
														   public ISSIMDamageableInterface
{
	GENERATED_BODY()
	
// Variables
#pragma region Components
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "CombatComponent")
	TObjectPtr<USSIMEnemyCombatComponent> EnemyCombatComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "StatsComponent")
	TObjectPtr<USSIMEnemyStatsComponent> EnemyStatsComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "DamageReactionComponent")
	TObjectPtr<USSIMEnemyDamageReactionComponent> EnemyDamageReactionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "AttackCollision")
	TObjectPtr<UBoxComponent> AttackBoxCollision;
	
#pragma endregion Components
	
#pragma region Metadata

	EFacingDirection EnemyFacingDirection;

#pragma endregion Metadata
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Data")
	TObjectPtr<USSIMEnemyDataAsset> EnemyData;
	
// Overriden Functions
public:
	ASSIMBaseEnemy();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	
// Custom Functions
#pragma region Inline Getters
public:
	UFUNCTION()
	FORCEINLINE UBoxComponent* GetAttackCollision() const
	{
		return AttackBoxCollision;
	}
	FORCEINLINE USSIMEnemyCombatComponent* GetEnemyCombatComponent() const
	{
		return EnemyCombatComponent;
	}
	FORCEINLINE USSIMEnemyStatsComponent* GetEnemyStatsComponent() const
	{
		return EnemyStatsComponent;
	}
	FORCEINLINE USSIMEnemyDamageReactionComponent* GetEnemyDamageReactionComponent() const
	{
		return EnemyDamageReactionComponent;
	}
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Enemy")
	FORCEINLINE EFacingDirection GetEnemyFacingDirection() const
	{
		return EnemyFacingDirection;
	}
#pragma endregion Inline Getters
	
public:
	void SetEnemyFacingDirection(EFacingDirection InPlayerFacingDirection);

protected:
	virtual void InitializeAttributes() override;
	void SetupAttackCollision();
	
private:
	
	void UpdateFacingDirection();
	
// Interfaces
public:
	virtual void StartAttackInterface_Implementation() const override;
	virtual void EndAttackInterface_Implementation() const override;
	virtual void StartAttackTraceInterface_Implementation() const override;
	virtual void EndAttackTraceInterface_Implementation() const override;
	
	virtual void ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const override;
	virtual void ReceivePlayerAttackDirectionType_Implementation(
		EPlayerAttackDirectionType InPlayerAttackDirectionType) const override;

	
};
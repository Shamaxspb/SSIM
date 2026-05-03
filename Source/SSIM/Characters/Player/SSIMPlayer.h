#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/DamageReaction/SSIMPlayerDamageReactionComponent.h"
#include "SSIM/Components/PlayerComponents/SSIMPlayerDashComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Core/Types/EPlayerState.h"
#include "SSIM/Core/Interfaces/PlayerDataInterface.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"

#include "SSIMPlayer.generated.h"

struct FInputActionValue;
class USSIMPlayerDamageReactionComponent;
class USSIMPlayerStatsComponent;
class UBoxComponent;
class UInputAction;
class USSIMPlayerCombatComponent;
class USSIMPlayerDashComponent;


UCLASS(meta = (PrioritizeCategories = "SSIM"))
class SSIM_API ASSIMPlayer : public ASSIMBaseCharacter, public IPlayerDataInterface,
														public ISSIMCombatInterface,
														public ISSIMDamageableInterface
{
	GENERATED_BODY()

// Variables	
#pragma region Components
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "CombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "StatsComponent")
	TObjectPtr<USSIMPlayerStatsComponent> SSIMPlayerStatsComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "DashComponent")
	TObjectPtr<USSIMPlayerDashComponent> SSIMPlayerDashComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "DamageReactionComponent")
	TObjectPtr<USSIMPlayerDamageReactionComponent> SSIMPlayerDamageReactionComponent;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Player|Player|Components", DisplayName = "AttackCollisionRoot", 
			  meta = (ToolTip = "This is blank component to group up attack collision components, just for clear hierarchy"))
	TObjectPtr<USceneComponent> RootAttackCollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Player|Player|Components", DisplayName = "FrontalAttackCollision")
	TObjectPtr<UBoxComponent> FrontalAttackCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Player|Player|Components", DisplayName = "UpperAttackCollision")
	TObjectPtr<UBoxComponent> UpperAttackCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Player|Player|Components", DisplayName = "BottomAttackCollision")
	TObjectPtr<UBoxComponent> BottomAttackCollision;
	
#pragma endregion Components
	
#pragma region Input
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input", DisplayName = "IA_Move")
	UInputAction* MoveInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input", DisplayName = "IA_Dash")
	UInputAction* DashInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input|Attack", DisplayName = "IA_Attack")
	UInputAction* AttackInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input|Attack", DisplayName = "IA_AttackUpward")
	UInputAction* AttackUpwardInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input|Attack", DisplayName = "IA_AttackUpward_Chord")
	UInputAction* UpwardAttackChord;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input|Attack", DisplayName = "IA_AttackDownward")
	UInputAction* AttackDownwardInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input|Attack", DisplayName = "IA_AttackDownward_Chord")
	UInputAction* DownwardAttackChord;

#pragma endregion Input	
	
protected:
	/*UPROPERTY()
	EPlayerState CurrentPlayerState;*/
	
#pragma region Player Defaults
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultPlayerGravityScale = 7.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultPlayerBrakingDecelerationWalking = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultHitRegistrationCollisionHalfHeight = 55.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultHitRegistrationCollisionRadius = 24.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultContactDamageCollisionHalfHeight = 75.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultContactDamageCollisionRadius = 35.f;
	
#pragma endregion  Player Defaults
	
	// Debug
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|DEBUG")
	bool bShowLogs;
	
private:
	float CachedPlayerRotationYaw;
	float MoveInputValue;
	
	
// Overriden Functions
public:ASSIMPlayer();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
// My Functions
#pragma region Inline Getters
	
public:
	// Component getters
	UFUNCTION()
	FORCEINLINE USSIMPlayerCombatComponent* GetPlayerCombatComponent() const
	{
		return SSIMPlayerCombatComponent;
	}
	UFUNCTION()
	FORCEINLINE USSIMPlayerDashComponent* GetPlayerFlowComponent() const
	{
		return SSIMPlayerDashComponent;
	}
	UFUNCTION()
	FORCEINLINE USSIMPlayerStatsComponent* GetPlayerStatsComponent() const
	{
		return SSIMPlayerStatsComponent;
	}
	UFUNCTION()
	FORCEINLINE USSIMPlayerDamageReactionComponent* GetPlayerDamageReactionComponent() const
	{
		return SSIMPlayerDamageReactionComponent;
	}
	
	UFUNCTION()
	FORCEINLINE UBoxComponent* GetFrontalAttackCollision() const
	{
		return FrontalAttackCollision;
	}
	UFUNCTION()
	FORCEINLINE UBoxComponent* GetUpperAttackCollision() const
	{
		return UpperAttackCollision;
	}
	UFUNCTION()
	FORCEINLINE UBoxComponent* GetBottomAttackCollision() const
	{
		return BottomAttackCollision;
	}
	
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|Input")
	FORCEINLINE float GetPlayerMoveInputValue() const
	{
		return MoveInputValue;
	}
	
	// State getters
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	FORCEINLINE bool GetIsPlayerAttacking() const
	{
		return SSIMPlayerCombatComponent->bAttacking;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	FORCEINLINE bool GetIsPlayerAttackKnockbackActive() const
	{
		return SSIMPlayerCombatComponent->bAttackKnockbackActive;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	FORCEINLINE bool GetIsPlayerPogoActive() const
	{
		return SSIMPlayerCombatComponent->bPogoActive;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	FORCEINLINE bool GetIsPlayerDashing() const
	{
		return SSIMPlayerDashComponent->bDashing;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	FORCEINLINE bool GetCanPlayerDash() const
	{
		return SSIMPlayerDashComponent->bCanDash;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	FORCEINLINE bool GetIsPlayerStaggered() const
	{
		return SSIMPlayerDamageReactionComponent->bStaggered;
	}
	FORCEINLINE bool GetIsPlayerInvulnerable() const
	{
		return SSIMPlayerStatsComponent->bInvulnerable;
	}
	
#pragma endregion Inline Getters
	
#pragma region Inline Setters
	
	FORCEINLINE void SetPlayerGravityScaleToDefault() const
	{
		GetCharacterMovement()->GravityScale = DefaultPlayerGravityScale;
	}
	FORCEINLINE void SetPlayerBrakingDecelerationWalkingToDefault() const
	{
		GetCharacterMovement()->BrakingDecelerationWalking = DefaultPlayerBrakingDecelerationWalking;
	}
	FORCEINLINE void SetContactDamageCollisionShapeDefault() const
	{
		ContactDamageCollision->SetCapsuleRadius(DefaultContactDamageCollisionRadius);
		ContactDamageCollision->SetCapsuleHalfHeight(DefaultContactDamageCollisionHalfHeight);
	}
	FORCEINLINE void SetContactDamageCollisionShapeDash() const
	{
		ContactDamageCollision->SetCapsuleRadius(
								GetContactDamageCollision()->GetScaledCapsuleHalfHeight(),
								true);
		ContactDamageCollision->SetCapsuleHalfHeight(DefaultContactDamageCollisionHalfHeight);
	}

#pragma endregion Inline Setters
	
private:
	void HandleMove(const FInputActionValue& Value);
	void HandleMoveCompleted();
	
	void SetupAttackCollision();
	
#pragma region Handler Functions
	
private:
	void HandleAttackFrontal();
	void HandleAttackUpward();
	void HandleAttackDownward();
	void HandleStartAttackTrace() const;
	void HandleEndAttackTrace() const;
	
	
	
	void HandleDash(); 
	
#pragma endregion Handler Functions

private:
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);
	
private:
	bool CanMove() const;
	bool CanAttack() const;
	bool CanDash() const;
	
// Interfaces
public:
	virtual void StartAttackInterface_Implementation() const override;
	virtual void EndAttackInterface_Implementation() const override;
	virtual void StartAttackTraceInterface_Implementation() const override;
	virtual void EndAttackTraceInterface_Implementation() const override;
	
	virtual void EndDashInterface_Implementation() const override;
	
	virtual void ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const override;

};

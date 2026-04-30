#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
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
class USSIMPlayerFlowComponent;


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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "FlowComponent")
	TObjectPtr<USSIMPlayerFlowComponent> SSIMPlayerFlowComponent;
	
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
	
#pragma region States
	
private:
	bool bAttacking  = false;
	bool bPogoActive = false;
	
	bool bDashing	 = false;
	bool bCanDash    = true;
	
	bool bStaggered  = false;
	
	
#pragma endregion States
	
#pragma region Player Defaults
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultPlayerGravityScale = 7.f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultPlayerBrakingDecelerationWalking = 10000.f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultContactDamageCollisionHalfHeight = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Defaults")
	float DefaultContactDamageCollisionRadius = 35.f;
	
#pragma endregion  Player Defaults
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|DEBUG")
	bool bShowLogs;
	
private:
	float CachedPlayerRotationYaw;
	
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
	FORCEINLINE USSIMPlayerFlowComponent* GetPlayerFlowComponent() const
	{
		return SSIMPlayerFlowComponent;
	}
	UFUNCTION()
	FORCEINLINE USSIMPlayerStatsComponent* GetPlayerStatsComponent() const
	{
		return SSIMPlayerStatsComponent;
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
	
	// State getters
	UFUNCTION()
	FORCEINLINE bool GetIsPlayerAttacking() const
	{
		return bAttacking;
	}
	UFUNCTION()
	FORCEINLINE bool GetIsPlayerPogoActive() const
	{
		return bPogoActive;
	}
	UFUNCTION()
	FORCEINLINE bool GetIsPlayerDashing() const
	{
		return bDashing;
	}
	UFUNCTION()
	FORCEINLINE bool GetCanPlayerDash() const
	{
		return bCanDash;
	}
	UFUNCTION()
	FORCEINLINE bool GetIsPlayerStaggered() const
	{
		return bStaggered;
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
	void BindToStateChangesInComponents() const;
	
#pragma region Handler Functions
	
private:
	void HandleAttackFrontal();
	void HandleAttackUpward();
	void HandleAttackDownward();
	void HandleStartAttackTrace();
	void HandleEndAttackTrace();
	
	void HandleDash(); 
	
#pragma endregion Handler Functions
	
#pragma region State Handlers
	
private:
	UFUNCTION()
	void OnAttackStartedHandler();
	UFUNCTION()
	void OnAttackEndedHandler();
	
	UFUNCTION()
	void OnPogoStartedHandler();
	UFUNCTION()
	void OnPogoEndedHandler();
	
	UFUNCTION()
	void OnDashStartedHandler();
	UFUNCTION()
	void OnDashEndedHandler();
	
	UFUNCTION()
	void OnCanDashStateChangedHandler(bool InCanDash);
	
	UFUNCTION()
	void OnStaggerStartedHandler();
	UFUNCTION()
	void OnStaggerEndedHandler();
	
#pragma endregion State Handlers
	
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

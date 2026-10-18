#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
#include "SSIM/Components/Combat/SSIMPlayerCombatComponent.h"
#include "SSIM/Components/DamageReaction/SSIMPlayerDamageReactionComponent.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"
#include "SSIM/Core/SSIMGameplayTags.h"
#include "SSIM/Core/Interfaces/PlayerDataInterface.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"
#include "SSIM/Core/Types/SSIMPlayerTypes.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "SSIM")
	TSubclassOf<UGameplayEffect> PlayerInitGE_temp;
	
// Variables	
#pragma region Components
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "CombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "StatsComponent")
	TObjectPtr<USSIMPlayerStatsComponent> SSIMPlayerStatsComponent;
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Player|Components", DisplayName = "DashComponent")
	TObjectPtr<USSIMPlayerDashComponent> SSIMPlayerDashComponent;*/
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Player|Input", DisplayName = "IA_Heal")
	UInputAction* HealInputAction;
	
#pragma endregion Input	
	
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
	
#pragma region Metadata
public:
	float MoveInputValue;
	
private:
	EFacingDirection PlayerFacingDirection;

#pragma endregion Metadata
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> InnateAbilities;
	
	
	// Debug
	UPROPERTY(EditDefaultsOnly, Category = "SSIM|Player|Debug")
	bool bShowPlayerInputLogs;
	
private:
	float CachedPlayerRotationYaw;
	
	
// Overriden Functions
public:ASSIMPlayer();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
// Custom Functions
#pragma region Inline Getters
public:
	// Component getters
	UFUNCTION()
	USSIMPlayerCombatComponent* GetPlayerCombatComponent() const
	{
		return SSIMPlayerCombatComponent;
	}
	UFUNCTION()
	USSIMPlayerStatsComponent* GetPlayerStatsComponent() const
	{
		return SSIMPlayerStatsComponent;
	}
	UFUNCTION()
	USSIMPlayerDamageReactionComponent* GetPlayerDamageReactionComponent() const
	{
		return SSIMPlayerDamageReactionComponent;
	}
	
	UFUNCTION()
	UBoxComponent* GetFrontalAttackCollision() const
	{
		return FrontalAttackCollision;
	}
	UFUNCTION()
	UBoxComponent* GetUpperAttackCollision() const
	{
		return UpperAttackCollision;
	}
	UFUNCTION()
	UBoxComponent* GetBottomAttackCollision() const
	{
		return BottomAttackCollision;
	}
	
	// Metadata getters
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|Input")
	float GetPlayerMoveInputValue() const
	{
		return MoveInputValue;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player")
	EFacingDirection GetPlayerFacingDirection() const
	{
		return PlayerFacingDirection;
	}
	float GetPlayerFacingDirectionValue() const
	{
		return PlayerFacingDirection == EFacingDirection::EPD_Right ? 1.f : -1.f;
	}
	
	// State getters
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	bool IsPlayerAttacking() const
	{
		return SSIMPlayerCombatComponent->bAttacking;
	}
	UFUNCTION(BlueprintPure, Category = "SSIM|Player|State")
	bool IsPlayerAttackKnockbackActive() const
	{
		return SSIMPlayerCombatComponent->bAttackKnockbackActive;
	}
	UFUNCTION(BlueprintPure, Category = "SSIM|Player|State")
	bool IsPlayerPogoActive() const
	{
		return SSIMPlayerCombatComponent->bPogoActive;
	}
	UFUNCTION(BlueprintCallable, Category = "SSIM|Player|State")
	bool IsPlayerHealing() const
	{
		return SSIMAbilitySystemComponent->HasMatchingGameplayTag(TAG_State_Healing.GetTag());
	}
	UFUNCTION(BlueprintPure, Category = "SSIM|Player|State")
	bool IsPlayerDashing() const
	{
		return SSIMAbilitySystemComponent->HasMatchingGameplayTag(TAG_State_Dashing.GetTag());
	}
	UFUNCTION(BlueprintPure, Category = "SSIM|Player|State")
	bool IsPlayerStaggered() const
	{
		return SSIMPlayerDamageReactionComponent->bStaggered;
	}
	UFUNCTION(BlueprintPure, Category = "SSIM|Player|State")
	bool IsPlayerInvulnerable() const
	{
		return SSIMPlayerStatsComponent->bInvulnerable;
	}
	UFUNCTION(BlueprintPure, Category = "SSIM|Player|State")
	bool IsPlayerDead() const
	{
		return SSIMPlayerStatsComponent->bDead;
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
#pragma endregion Inline Setters
	
public:
	void SetPlayerFacingDirection(EFacingDirection InPlayerFacingDirection);
	
protected:
	virtual void InitializeAttributes() override;
	virtual void InitializeAbilities() override;

private:
	void SetupAttackCollision();
	
#pragma region Handler Functions
	
private:
	void HandleMove(const FInputActionValue& Value);
	void HandleMoveCompleted();
	
	void HandleDash(); 
	
	void HandleAttackFrontal();
	void HandleAttackUpward();
	void HandleAttackDownward();
	void HandleStartAttackTrace() const;
	void HandleEndAttackTrace() const;
	
	void HandleHeal(); 
	
#pragma endregion Handler Functions

private:
	UFUNCTION()
	void OnDamageReceivedHandler(const FDamageData& InDamageData);
	
	bool CanMove();
	bool CanAttack() const;
	
	
// Interfaces
public:
	virtual void StartAttackInterface_Implementation() const override;
	virtual void EndAttackInterface_Implementation() const override;
	virtual void StartAttackTraceInterface_Implementation() const override;
	virtual void EndAttackTraceInterface_Implementation() const override;
	
	virtual void ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const override;

	// Debug
protected:
	UFUNCTION(BlueprintCallable, Category = "SSIM|Debug")
	void UnlockInnateAbility_DEBUG(FGameplayTag InAbilityTag);
};

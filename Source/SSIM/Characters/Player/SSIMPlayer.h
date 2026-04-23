#pragma once

#include "CoreMinimal.h"
#include "SSIM/Characters/SSIMBaseCharacter.h"
#include "SSIM/Core/Types/EPlayerState.h"
#include "SSIM/Core/Interfaces/PlayerDataInterface.h"
#include "SSIM/Core/Interfaces/SSIMCombatInterface.h"
#include "SSIM/Core/Interfaces/SSIMDamageableInterface.h"

#include "SSIMPlayer.generated.h"

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "CombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "StatsComponent")
	TObjectPtr<USSIMPlayerStatsComponent> SSIMPlayerStatsComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "FlowComponent")
	TObjectPtr<USSIMPlayerFlowComponent> SSIMPlayerFlowComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "DamageReactionComponent")
	TObjectPtr<USSIMPlayerDamageReactionComponent> SSIMPlayerDamageReactionComponent;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "AttackCollisionRoot", 
			  meta = (ToolTip = "This is blank component to group up attack collision components, just for clear hierarchy"))
	TObjectPtr<USceneComponent> RootAttackCollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "FrontalAttackCollision")
	TObjectPtr<UBoxComponent> FrontalAttackCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "UpperAttackCollision")
	TObjectPtr<UBoxComponent> UpperAttackCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "BottomAttackCollision")
	TObjectPtr<UBoxComponent> BottomAttackCollision;
	
#pragma endregion Components
	
#pragma region Input
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input", DisplayName = "IA_MoveRight")
	UInputAction* MoveRightInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input", DisplayName = "IA_MoveLeft")
	UInputAction* MoveLeftInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input", DisplayName = "IA_Dash")
	UInputAction* DashInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input|Attack", DisplayName = "IA_Attack")
	UInputAction* AttackInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input|Attack", DisplayName = "IA_AttackUpward")
	UInputAction* AttackUpwardInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input|Attack", DisplayName = "IA_AttackUpward_Chord")
	UInputAction* UpwardAttackChord;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input|Attack", DisplayName = "IA_AttackDownward")
	UInputAction* AttackDownwardInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input|Attack", DisplayName = "IA_AttackDownward_Chord")
	UInputAction* DownwardAttackChord;
	
#pragma endregion Input	
	
protected:
	UPROPERTY()
	EPlayerState CurrentPlayerState;

private:
	float ContactDamageCollisionDefaultRadius = 22.f;
	
	
// Overriden Functions
public:
	ASSIMPlayer();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
// My Functions
#pragma region Inline Getters
	
public:
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
	
#pragma endregion Inline Getters
	
private:
	void MoveRight();
	void MoveLeft();
	
	void SetupAttackCollision();
	
#pragma region Handler Functions
	
private:
	void HandleAttackFrontal();
	void HandleAttackUpward();
	void HandleAttackDownward();
	void HandleStartAttackTrace();
	void HandleEndAttackTrace();
	
	void HandleDash(); 
	
#pragma endregion Handler Functions
	
private:
	UFUNCTION()
	void OnEndEndInvulnerabilityCollisionUpdate();
	
	UFUNCTION()
	void StartDashHandler();
	UFUNCTION()
	void EndDashHandler();
	
// Interfaces
public:
	virtual void StartAttackInterface_Implementation() const override;
	virtual void EndAttackInterface_Implementation() const override;
	virtual void StartAttackTraceInterface_Implementation() const override;
	virtual void EndAttackTraceInterface_Implementation() const override;
	
	virtual void EndDashInterface_Implementation() const override;
	
	virtual void ReceiveDamageInterface_Implementation(const FDamageData& InDamageData) const override;

};

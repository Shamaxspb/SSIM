#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSIM/Core/Types/EPlayerState.h"
#include "../Core/Interfaces/PlayerDataInterface.h"

#include "SSIMPlayer.generated.h"

class UBoxComponent;
class UInputAction;
class USSIMPlayerCombatComponent;
class USSIMPlayerFlowComponent;

UCLASS()
class SSIM_API ASSIMPlayer : public ACharacter, public IPlayerDataInterface
{
	GENERATED_BODY()

// Variables
protected:
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerCombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerFlowComponent")
	TObjectPtr<USSIMPlayerFlowComponent> SSIMPlayerFlowComponent;
	
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
	
	UPROPERTY()
	EPlayerState CurrentPlayerState;
	
	
// Overriden Functions
public:
	ASSIMPlayer();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
		
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
// My Functions
public:
#pragma region Inline Getters
	
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
	
	void HandleDash(); 
	
	void SetupAttackCollision();
	
	void HandleAttackFrontal();
	void HandleAttackUpward();
	void HandleAttackDownward();
	
	void HandleStartAttackTrace();
	void HandleEndAttackTrace();
	
	
// Interfaces
public:
	virtual USSIMPlayerCombatComponent* GetPlayerCombatComponentInterface_Implementation() const override;
	
	virtual USSIMPlayerFlowComponent* GetPlayerFlowComponentInterface_Implementation() const override;
	
};

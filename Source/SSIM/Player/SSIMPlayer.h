#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSIM/Core/Types/EPlayerState.h"
#include "Components/SSIMPlayerCombatComponent.h"
#include "Components/SSIMPlayerFlowComponent.h"
#include "../Core/Interfaces/PlayerDataInterface.h"

#include "SSIMPlayer.generated.h"

class UInputAction;

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
#pragma endregion Components
	
#pragma region Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input", DisplayName = "IA_MoveRight")
	UInputAction* MoveRightInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input", DisplayName = "IA_MoveLeft")
	UInputAction* MoveLeftInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Input", DisplayName = "IA_Dash")
	UInputAction* DashInputAction;
	
#pragma endregion Input
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TArray<UAnimMontage*> PlayerAttackAnimations;
	
	
	UPROPERTY(VisibleAnywhere, Category = "SSIM|Metadata")
	EPlayerState CurrentPlayerState;
	
	
// Overriden Functions
public:
	ASSIMPlayer();

	virtual void Tick(float DeltaTime) override;
		
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;
	
	
// My Functions
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
	
private:
	void MoveRight();
	
	void MoveLeft();
	
	void HandleDash(); 
	
// Interfaces
	// This entire thing is just practice, and it is an overengineering, just get all this data in ActorComponent directly
	virtual TArray<UAnimMontage*> GetAttackAnimationsInterface_Implementation() const override final;
	
	virtual UAnimMontage* GetRandomAttackAnimationInterface_Implementation() const override final;
	// ------------------ end of overengineering
};

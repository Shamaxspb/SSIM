#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSIM/Core/Types/EPlayerState.h"
#include "Components/SSIMPlayerCombatComponent.h"
#include "Components/SSIMPlayerFlowComponent.h"
#include "../Core/Interfaces/PlayerDataInterface.h"

#include "SSIMPlayer.generated.h"

UCLASS()
class SSIM_API ASSIMPlayer : public ACharacter, public IPlayerDataInterface
{
	GENERATED_BODY()

// Variables
protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerCombatComponent")
	TObjectPtr<USSIMPlayerCombatComponent> SSIMPlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SSIM|Components", DisplayName = "PlayerFlowComponent")
	TObjectPtr<USSIMPlayerFlowComponent> SSIMPlayerFlowComponent;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SSIM|Animations")
	TObjectPtr<UAnimMontage> PlayerDashAnimation;
	
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
	
// Interfaces
	// This entire thing is just practice, and it is an overengineering, just get all this data in ActorComponent directly
	virtual UAnimMontage* GetDashAnimationInterface() override;
	
	virtual TArray<UAnimMontage*> GetAttackAnimationsInterface() override;
	
	virtual UAnimMontage* GetRandomAttackAnimationInterface() override;
	// ------------------ end of overengineering
};

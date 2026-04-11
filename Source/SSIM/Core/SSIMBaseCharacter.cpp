// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SSIM/SSIM.h"

// Overriden Functions
ASSIMBaseCharacter::ASSIMBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RestrictXAxisMovement();

}

// My Functions
void ASSIMBaseCharacter::RestrictXAxisMovement() const
{
	if (!GetMovementComponent())
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("%s : GetMovementComponent is NULL"), TEXT(__FUNCTION__));
	}
	
	GetMovementComponent()->SetPlaneConstraintEnabled(true);
	GetMovementComponent()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
	
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s | Plane Constraint Axis Setting: %s (%s)"),
											 TEXT(__FUNCTION__), 
											 *UEnum::GetValueAsString(GetMovementComponent()->GetPlaneConstraintAxisSetting()),
											 *GetName());
}

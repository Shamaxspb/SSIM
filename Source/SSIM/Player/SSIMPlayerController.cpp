// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerController.h" 
#include "SSIMPlayer.h" 
#include "SSIM/SSIM.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"


// Overriden Functions
ASSIMPlayerController::ASSIMPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASSIMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
	
}

// OnPossess called TWICE for some reason
// It doesn't affect anything yet, but should be considered in the future
void ASSIMPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	GetSSIMPlayerReference(InPawn);
	
}

void ASSIMPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// My Functions
void ASSIMPlayerController::Init()
{
	InitBasicInputContext();
	
}

void ASSIMPlayerController::PerformDash() const
{
	if (!SSIMPlayer)
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("PerformDash: SSIMPlayer is not valid"));
		return;
	}
	
	if (!IsValid(SSIMPlayer->GetPlayerFlowComponent()))
	{
		UE_LOG(LogSSIMValidations, Error, TEXT("PerformDash: Couldn't receive %s PlayerFlowComponent"), *SSIMPlayer->GetPlayerFlowComponent()->GetName());
		return;
	}
	
	SSIMPlayer->GetPlayerFlowComponent()->UseDash();
	
}

void ASSIMPlayerController::GetSSIMPlayerReference(APawn* InPawn)
{
	if (!InPawn)
	{
		UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("Possessed pawn is not valid"));
	}
	
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s: InPawn = %s"), *this->GetName(), *InPawn->GetName());

	SSIMPlayer = CastChecked<ASSIMPlayer>(InPawn);

	if (!IsValid(SSIMPlayer))
	{
		UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("Failed cast PlayerPawn to SSIMPlayer %s"), *InPawn->GetName());
	}
	
}

void ASSIMPlayerController::InitBasicInputContext()
{
	if (!GetLocalPlayer())
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("LocalPlayer is not valid"));
		return;
	}
	
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputSubsystem)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("EnhancedInputSubsystem is not valid"));
		return;
	}
	
	EnhancedInputSubsystem->AddMappingContext(BaseInputContext, 1);
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("EnhancedInputSubsystem %s added"), *BaseInputContext->GetName());
	
}


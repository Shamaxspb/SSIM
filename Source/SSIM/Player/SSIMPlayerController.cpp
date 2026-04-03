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
	
	SetSSIMPlayerReference(InPawn);
	SetSSIMActorComponentsReferences();
	
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

void ASSIMPlayerController::SetSSIMPlayerReference(APawn* InPawn)
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
		return;
	}
	
}

// Lost its purpose, won't delete for now
void ASSIMPlayerController::SetSSIMActorComponentsReferences()
{
	if (!SSIMPlayer)
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("SSIMPlayer is not valid"));
		return;
	}
	
	if (!IsValid(SSIMPlayer->GetPlayerFlowComponent()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("SSIMPlayerFlowComponent is not valid"))
	}
	SSIMPlayerFlowComponent = SSIMPlayer->GetPlayerFlowComponent();
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s is set"), *SSIMPlayerFlowComponent->GetName());
	
	if (!IsValid(SSIMPlayer->GetPlayerCombatComponent()))
	{
		UE_LOG(LogSSIMPlayerInitialization, Error, TEXT("SSIMPlayerCombatComponent is not valid"))
	}
	SSIMPlayerCombatComponent = SSIMPlayer->GetPlayerCombatComponent();
	UE_LOG(LogSSIMPlayerInitialization, Log, TEXT("%s is set"), *SSIMPlayerCombatComponent->GetName());
	
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMPlayerDamageReactionComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SSIM/Components/Stats/SSIMPlayerStatsComponent.h"


// Overriden functions
USSIMPlayerDamageReactionComponent::USSIMPlayerDamageReactionComponent()
{
}

void USSIMPlayerDamageReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();
	StaggeredFirstFrame->BlendIn = StaggeredFirstFrameBlendInTime;
}

// My Functions
void USSIMPlayerDamageReactionComponent::SetReferences()
{
	Super::SetReferences();
	
	PlayerStatsComponent = SSIMOwnerCharacter->FindComponentByClass<USSIMPlayerStatsComponent>();
}

void USSIMPlayerDamageReactionComponent::InitStagger()
{
	SSIMOwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	SSIMOwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
	SSIMOwnerCharacter->PlayAnimMontage(StaggeredFirstFrame, 0.f);
	PlayerStatsComponent->bStaggered = true;
	PlayerStatsComponent->bInvulnerable = true;
}

void USSIMPlayerDamageReactionComponent::StartStaggerSequence()
{
	CurrentStaggerSequenceStep = 0;
	
	/*Steps = {
		{ StaggeredFirstFrameBlendInTime, [this]() {StartStopFrame();}},
		{StopFrameDuration * 0.01, [this]() {StopFrameToStagger();}},
		{StaggerDuration, [this]() {EndStagger();}},
		{InvulnerabilityDuration, [this](){EndInvulnerability(); }}
	};*/
}

void USSIMPlayerDamageReactionComponent::StartStopFrame() const
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
	
}

void USSIMPlayerDamageReactionComponent::StopFrameToStagger()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

	FVector ReboundLaunchVelocity = FVector(0.0f, SSIMOwnerCharacter->GetActorForwardVector().Y * ReboundVelocityY * -1.f, ReboundVelocityZ);
	SSIMOwnerCharacter->LaunchCharacter(ReboundLaunchVelocity, true, true);
	
	SSIMOwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
}

void USSIMPlayerDamageReactionComponent::EndStagger()
{
	PlayerStatsComponent->bStaggered = false;
	SSIMOwnerCharacter->StopAnimMontage(StaggeredAnimation);
}

void USSIMPlayerDamageReactionComponent::EndInvulnerability()
{
	PlayerStatsComponent->bInvulnerable = false;
}

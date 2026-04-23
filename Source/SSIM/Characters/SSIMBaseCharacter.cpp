// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SSIM/SSIM.h"

// Overriden Functions
ASSIMBaseCharacter::ASSIMBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RestrictXAxisMovement();
	SetupDamageRegistrationCollision();

}

void ASSIMBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// Restrict X axis editor placement
	FVector Location = GetActorLocation();
	Location.X = 0.f;
	SetActorLocation(Location);
	
	// Set rotation
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw = -90.f;
	SetActorRotation(Rotation);
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

void ASSIMBaseCharacter::SetupDamageRegistrationCollision()
{
	DamageRegistrationGroup = CreateDefaultSubobject<USceneComponent>(TEXT("DamageRegistrationGroup"));
	DamageRegistrationGroup->SetupAttachment(GetRootComponent());
	
	
	HitRegistrationCollision	= CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitRegistrationCollision"));
	HitRegistrationCollision->ComponentTags.Add("HitRegistrationCollision");
	
	ContactDamageCollision	= CreateDefaultSubobject<UCapsuleComponent>(TEXT("ContactDamageCollision"));
	ContactDamageCollision->ComponentTags.Add("ContactDamageCollision");
	

	TArray<TObjectPtr<UCapsuleComponent>> DamageRegistrationCollisions;
	DamageRegistrationCollisions.Add(HitRegistrationCollision);
	DamageRegistrationCollisions.Add(ContactDamageCollision);
	
	for (auto const Element : DamageRegistrationCollisions)
	{
		Element->SetupAttachment(DamageRegistrationGroup);
		Element->SetGenerateOverlapEvents(true);
		Element->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	
	HitRegistrationCollision->SetCollisionProfileName("HitRegistration", true);
	ContactDamageCollision->SetCollisionProfileName("ContactDamage", true);

}



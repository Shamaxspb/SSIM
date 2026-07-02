// Fill out your copyright notice in the Description page of Project Settings.


#include "SSIMBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SSIM/SSIM.h"
#include "SSIM/Core/GAS/SSIMAttributeSet.h"

// Overriden Functions
ASSIMBaseCharacter::ASSIMBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SSIMAbilitySystemComponent = CreateDefaultSubobject<USSIMAbilitySystemComponent>(TEXT("SSIMAbilitySystemComponent"));
	SSIMAbilitySystemComponent->SetIsReplicated(false);
	
	SSIMAttributeSet = CreateDefaultSubobject<USSIMAttributeSet>(TEXT("SSIMAttributeSet"));
	
	
	RestrictXAxisMovement();
	SetupDamageRegistrationCollision();

}

void ASSIMBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SSIMAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ASSIMBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// Restrict X axis editor placement
	FVector Location = GetActorLocation();
	Location.X = 0.f;
	SetActorLocation(Location);
}

USSIMAbilitySystemComponent* ASSIMBaseCharacter::GetAbilitySystemComponent() const
{
	return SSIMAbilitySystemComponent;	
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



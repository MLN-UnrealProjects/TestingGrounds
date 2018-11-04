// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "../Weapons/Gun.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
// Sets default values

AMannequin::AMannequin() :
	Mesh1P{ CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P")) },
	FirstPersonCameraComponent{ CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera")) }
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-1.55f, -4.25f, 62.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (!ensure(GunBlueprint))
	{
		return;
	}

	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	if (IsPlayerControlled())
	{
		Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GripPointSocketName); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GripPointSocketName); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	}
	Gun->FPAnimInstance = Mesh1P->GetAnimInstance();
	Gun->TPAnimInstance = GetMesh()->GetAnimInstance();

	if (InputComponent)
	{
		InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

void AMannequin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Gun)
	{
		Gun->Destroy();
	}
}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();

	if (Gun)
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GripPointSocketName);
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AMannequin::PullTrigger()
{
	if (Gun)
		Gun->OnFire();
}
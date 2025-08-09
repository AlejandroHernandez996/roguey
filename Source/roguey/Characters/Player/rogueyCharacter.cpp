// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Grid/Util/GridUtils.h"
#include "DrawDebugHelpers.h"
#include "rogueyGameMode.h"

ArogueyCharacter::ArogueyCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create the camera component
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ArogueyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// stub
}

void ArogueyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TrueTileQueue.IsEmpty())
	{
		DrawTrueTile(Cast<ArogueyGameMode>(GetWorld()->GetAuthGameMode())->GridManager->Grid.ActorMapLocation[this],DeltaSeconds);
		return;
	}
	TPair<FIntVector2, float> TargetTrueTile = *TrueTileQueue.Peek();
	FVector TargetLocation = GridUtils::GridToWorld(TargetTrueTile.Key);
	FVector CurrentLocation = GetActorLocation();

	TargetLocation.Z = CurrentLocation.Z;

	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	UE_LOG(LogTemp, Log, TEXT("Current Location: %s, Target Location: %s"), *CurrentLocation.ToString(), *TargetLocation.ToString());
	float BaseSpeed = 100.0f;
	float ScaleFactor = 0.1f;
	float SpeedMultiplier = 1.0f + (QueueSize - 1) * ScaleFactor;
	float MovementSpeed = BaseSpeed * TargetTrueTile.Value / 0.6f * SpeedMultiplier;
	

	FVector MovementStep = Direction * MovementSpeed * DeltaSeconds;

	if (FVector::Dist(CurrentLocation, TargetLocation) > 3.0f)
	{
		SetActorLocation(CurrentLocation + MovementStep);
		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
	else
	{
		SetActorLocation(TargetLocation);
		if (!TrueTileQueue.IsEmpty())
		{
			TrueTileQueue.Pop();
			QueueSize--;
		}
	}
}

void ArogueyCharacter::DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime)
{
	FVector WorldTileCenter = GridUtils::GridToWorld(TrueTileLocation);

	// Calculate corner offsets assuming each tile is 100x100
	float HalfTileSize = 50.0f; // Half of 100
	float ZOffset = 1.0f;       // Small offset to draw above the mesh

	FVector CornerOffsets[] = {
		FVector(HalfTileSize, HalfTileSize, 0),    // Top Right
		FVector(-HalfTileSize, HalfTileSize, 0),   // Top Left
		FVector(HalfTileSize, -HalfTileSize, 0),   // Bottom Right
		FVector(-HalfTileSize, -HalfTileSize, 0)   // Bottom Left
	};

	FVector CornerHeights[4];

	for (int32 i = 0; i < 4; i++)
	{
		FVector CornerLocation = WorldTileCenter + CornerOffsets[i];
		FVector StartLocation = CornerLocation + FVector(0, 0, 1000);
		FVector EndLocation = CornerLocation - FVector(0, 0, 2000); 

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility);

		if (bHit)
		{
			CornerHeights[i] = HitResult.Location + FVector(0, 0, ZOffset);
		}
		else
		{
			CornerHeights[i] = CornerLocation + FVector(0, 0, ZOffset);
		}
	}

	DrawDebugLine(GetWorld(), CornerHeights[0], CornerHeights[1], FColor::Yellow, false, DecayTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[1], CornerHeights[3], FColor::Yellow, false, DecayTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[3], CornerHeights[2], FColor::Yellow, false, DecayTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[2], CornerHeights[0], FColor::Yellow, false, DecayTime, 0, 2.0f);
}

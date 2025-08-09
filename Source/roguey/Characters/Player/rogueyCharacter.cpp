// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Grid/Util/GridUtils.h"
#include "DrawDebugHelpers.h"
#include "rogueyGameMode.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

ArogueyCharacter::ArogueyCharacter()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(Mesh);
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
	SetPawnState(EPawnState::IDLE);
	// stub
}

void ArogueyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000.0f); // Cast downwards

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, // or another appropriate collision channel
		Params
	);
	if (bHit)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = HitResult.Location.Z + 100.0f; // Adjust for your specific height
		SetActorLocation(NewLocation);
	}
	
	DrawTrueTile(Cast<ArogueyGameMode>(GetWorld()->GetAuthGameMode())->GridManager->Grid.ActorMapLocation[this],DeltaSeconds*2.0f);

	if (TrueTileQueue.IsEmpty())
	{
		SetPawnState(EPawnState::IDLE);
		return;
	}
	TPair<FIntVector2, float> TargetTrueTile = *TrueTileQueue.Peek();
	FVector TargetLocation = GridUtils::GridToWorld(TargetTrueTile.Key);
	FVector CurrentLocation = GetActorLocation();

	TargetLocation.Z = CurrentLocation.Z;

	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	float BaseSpeed = 100.0f;
	float MovementSpeed = BaseSpeed * TargetTrueTile.Value / 0.6f;
	UE_LOG(LogTemp, Log, TEXT("Distance: %f") ,TargetTrueTile.Value);


	FVector MovementStep = Direction * MovementSpeed * DeltaSeconds;

	if (FVector::Dist(CurrentLocation, TargetLocation) > 5.0f)
	{
		SetActorLocation(CurrentLocation + MovementStep);
		if (TargetTrueTile.Value >= 2.0f)
		{
			SetPawnState(EPawnState::RUNNING);
		}else
		{
			SetPawnState(EPawnState::WALKING);
		}
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

void ArogueyCharacter::SetPawnState(EPawnState State)
{
	if (State == PawnState) return;
	PawnState = State;
	switch (State)
	{
	case EPawnState::IDLE:
		PlayAnimMontage(IdleMontage);
		break;
	case EPawnState::RUNNING:
		PlayAnimMontage(RunMontage);
		break;
	case EPawnState::WALKING:
		PlayAnimMontage(WalkMontage);
		break;
	}
}
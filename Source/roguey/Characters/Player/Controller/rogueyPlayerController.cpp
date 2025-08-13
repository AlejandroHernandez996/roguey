// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "InteractTypeArray.h"
#include "rogueyGameMode.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grid/Util/GridUtils.h"
#include "Input/Input.h"
#include "Input/rogueyInputManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ArogueyPlayerController::ArogueyPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ArogueyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	RogueyGameMode = Cast<ArogueyGameMode>(GetWorld()->GetAuthGameMode());
	if (GetPawn())
	{
		CameraBoom = GetPawn()->FindComponentByClass<USpringArmComponent>();
		FollowCamera = GetPawn()->FindComponentByClass<UCameraComponent>();
	}
}

void ArogueyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_WorldStatic, true, Hit))
	{
		DrawHoveredTile(Hit.Location);
	}

	FHitResult InteractHit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1, true, InteractHit);
	AActor* HitActor = InteractHit.GetActor();
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable* Interactable = Cast<IInteractable>(HitActor);
		if (Interactable && !Interactable->GetInteractList().IsEmpty())
		{
			OnHoverEvent.Broadcast(EInteractTypeToString(Interactable->GetInteractList()[0]), "<Yellow>"+Interactable->GetRogueyName()+"</>");
			return;
		}
	}
	if (GetHitResultUnderCursor(ECC_WorldStatic, true, Hit))
	{
		OnHoverEvent.Broadcast("Walk Here", "");
	}else
	{
		OnHoverEvent.Broadcast("", "");
	}
}

void ArogueyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ArogueyPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ArogueyPlayerController::OnInputTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ArogueyPlayerController::OnInputReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ArogueyPlayerController::OnInputReleased);
		EnhancedInputComponent->BindAction(InteractMenuAction, ETriggerEvent::Started, this, &ArogueyPlayerController::OnInteractMenuStarted);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ArogueyPlayerController::OnZoomTriggered);
		EnhancedInputComponent->BindAction(MouseScrollClickAction, ETriggerEvent::Started, this, &ArogueyPlayerController::OnMouseScrollStarted);
		EnhancedInputComponent->BindAction(MouseScrollClickAction, ETriggerEvent::Completed, this, &ArogueyPlayerController::OnMouseScrollReleased);
		EnhancedInputComponent->BindAction(MouseScrollClickAction, ETriggerEvent::Canceled, this, &ArogueyPlayerController::OnMouseScrollReleased);
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &ArogueyPlayerController::OnMouseMove);
	}
}

void ArogueyPlayerController::OnInputStarted()
{
	bIsInteractMenuOpen = false;
	FHitResult WorldHit;
	FHitResult InteractHit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1, true, InteractHit);
	AActor* HitActor = InteractHit.GetActor();
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable* Interactable = Cast<IInteractable>(HitActor);
		if (Interactable && HitActor != Cast<ArogueyCharacter>(GetPawn()))
		{
			OnClickEvent.Broadcast(false);
			const FInput Input(RogueyGameMode->GetCurrentTick(), EInputType::ATTACK, InteractHit.Location, Cast<ArogueyPawn>(GetPawn()), Cast<ArogueyPawn>(HitActor));
			RogueyGameMode->InputManager->EnqueueInput(Input);
			return;
		}
	}
	if (GetHitResultUnderCursor(ECC_WorldStatic, true, WorldHit))
	{
		const FInput Input(RogueyGameMode->GetCurrentTick(), EInputType::MOVE, WorldHit.Location, Cast<ArogueyPawn>(GetPawn()), nullptr);
		RogueyGameMode->InputManager->EnqueueInput(Input);
		OnClickEvent.Broadcast(true);
	}
}

void ArogueyPlayerController::OnInputTriggered()
{
}

void ArogueyPlayerController::OnInputReleased()
{
}

void ArogueyPlayerController::OnInteractMenuStarted()
{
	bIsInteractMenuOpen = true;
	InteractMenuEntries.Empty();
	
	FHitResult WorldHit;
	GetHitResultUnderCursor(ECC_WorldStatic, true, WorldHit);
	InteractMenuLocation = WorldHit.Location;
	FHitResult InteractHit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1, true, InteractHit);
	AActor* HitActor = InteractHit.GetActor();
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable* Interactable = Cast<IInteractable>(HitActor);
		if (Interactable && HitActor != Cast<ArogueyCharacter>(GetPawn()))
		{
			FInteractTypeArray InteractActorArray;
			InteractActorArray.InteractableActor = HitActor;
			InteractActorArray.Array = Interactable->GetInteractList();
			InteractActorArray.Name = Interactable->GetRogueyName();
			InteractMenuEntries.Add(InteractActorArray);
		}
	}
	
	OnInteractMenuEvent.Broadcast();
}

void ArogueyPlayerController::OnZoomTriggered(const FInputActionInstance& Instance)
{
	float ZoomValue = Instance.GetValue().Get<float>();

	CameraBoom->TargetArmLength = FMath::Clamp(
		CameraBoom->TargetArmLength + -ZoomValue * 50.0f,
		100.0f,
		1300.0f
	);
}

void ArogueyPlayerController::OnMouseScrollStarted()
{
	bIsRotating = true;
}

void ArogueyPlayerController::OnMouseScrollReleased()
{
	bIsRotating = false;
}

void ArogueyPlayerController::OnMouseMove(const FInputActionInstance& Instance)
{
	float RotationSpeed = 3.0f;
	float MinPitch = -80.0f;
	float MaxPitch = -15.0f;
	
	if (bIsRotating && CameraBoom)
	{
		FVector2D MoveValue = Instance.GetValue().Get<FVector2D>();
		FRotator NewRotation = CameraBoom->GetComponentRotation();
		NewRotation.Yaw += MoveValue.X * RotationSpeed;
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - MoveValue.Y * -RotationSpeed, MinPitch, MaxPitch);

		CameraBoom->SetWorldRotation(NewRotation);
	}
}

void ArogueyPlayerController::DrawHoveredTile(const FVector& HoveredPosition)
{
	FIntVector2 TileLocation = GridUtils::WorldToGrid(HoveredPosition);
	FVector WorldTileCenter = GridUtils::GridToWorld(TileLocation);

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

	DrawDebugLine(GetWorld(), CornerHeights[0], CornerHeights[1], FColor::White, false, .05f, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[1], CornerHeights[3], FColor::White, false, .05f, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[3], CornerHeights[2], FColor::White, false, .05f, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[2], CornerHeights[0], FColor::White, false, .05f, 0, 2.0f);
}

void ArogueyPlayerController::InteractMenuInput(AActor* InputActor, EInteractType InteractType,
	FVector InteractLocation)
{
	if (InteractType == EInteractType::EXAMINE)
	{
		IInteractable* Interactable = Cast<IInteractable>(InputActor);
		if (Interactable)
		{
			OnChatMessage.Broadcast(Interactable->GetExamineText());
		}
	}
}

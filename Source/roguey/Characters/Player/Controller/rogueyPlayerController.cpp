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
#include "ViewportInteractionTypes.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grid/Util/GridUtils.h"
#include "Input/Input.h"
#include "Input/rogueyInputManager.h"
#include "Inventory/InventoryEvent.h"
#include "Inventory/rogueyInventoryManager.h"
#include "Items/rogueyItemActor.h"
#include "Kismet/GameplayStatics.h"

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
	RogueyGameMode->InventoryManager->RogueyPlayerController = this;
}

void ArogueyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsUIOnly) return;
	
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_WorldStatic, true, Hit))
	{
		DrawHoveredTile(Hit.Location);
	}
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(GetPawn());
	FVector WorldOrigin;
	FVector WorldDirection;
	FVector2D MousePosition;
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	LocalPlayer->ViewportClient->GetMousePosition(MousePosition);
	UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection);
	GetWorld()->LineTraceMultiByChannel(HitResults, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, Params);

	FHitResult InteractHit;
	if (HitResults.Num() > 0)
	{
		InteractHit = HitResults[0];
	}

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
	bool bHitWorld = GetHitResultUnderCursor(ECC_WorldStatic, true, WorldHit);
	FHitResult InteractHit;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(GetPawn());
	FVector WorldOrigin;
	FVector WorldDirection;
	FVector2D MousePosition;
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	LocalPlayer->ViewportClient->GetMousePosition(MousePosition);
	UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection);
	GetWorld()->LineTraceMultiByChannel(HitResults, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, Params);

	if (HitResults.Num() > 0)
	{
		InteractHit = HitResults[0];
	}
	AActor* HitActor = InteractHit.GetActor();
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable* Interactable = Cast<IInteractable>(HitActor);
		if (Interactable && HitActor != Cast<ArogueyCharacter>(GetPawn()))
		{
			FInput Input;
			ArogueyPawn* TargetPawn = Cast<ArogueyPawn>(HitActor);
			ArogueyItemActor* TargetItem = Cast<ArogueyItemActor>(HitActor);
			if (TargetPawn && TargetPawn->PawnState != EPawnState::DEAD)
			{
				Input= FInput(RogueyGameMode->GetCurrentTick(), EInputType::ATTACK, InteractHit.Location, Cast<ArogueyPawn>(GetPawn()), TargetPawn);
				RogueyGameMode->InputManager->EnqueueInput(Input);
				OnClickEvent.Broadcast(false);
				return;
			}
			if (TargetItem)
			{
				Input= FInput(RogueyGameMode->GetCurrentTick(), EInputType::PICK_UP_ITEM, Cast<ArogueyPawn>(GetPawn()), TargetItem);
				RogueyGameMode->InputManager->EnqueueInput(Input);
				OnClickEvent.Broadcast(false);
				return;
			}
		}
	}
	if (bHitWorld)
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
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(GetPawn());
	FVector WorldOrigin;
	FVector WorldDirection;
	FVector2D MousePosition;
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	LocalPlayer->ViewportClient->GetMousePosition(MousePosition);
	UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection);
	GetWorld()->LineTraceMultiByChannel(HitResults, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, Params);
	
	for (FHitResult HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
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

	float HalfTileSize = 50.0f; // Half of 100
	float ZOffset = 1.0f; 

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

void ArogueyPlayerController::InteractMenuInput(AActor* InputActor, EInteractType InteractType)
{
	if (InteractType == EInteractType::EXAMINE)
	{
		IInteractable* Interactable = Cast<IInteractable>(InputActor);
		if (Interactable)
		{
			OnChatMessage.Broadcast(Interactable->GetExamineText());
		}
	}
	if (InteractType == EInteractType::WALK)
	{
		const FInput Input(RogueyGameMode->GetCurrentTick(), EInputType::MOVE, InteractMenuLocation, Cast<ArogueyPawn>(GetPawn()), nullptr);
		RogueyGameMode->InputManager->EnqueueInput(Input);
	}
	if (InteractType == EInteractType::ATTACK)
	{
		const FInput Input(RogueyGameMode->GetCurrentTick(), EInputType::ATTACK, FVector::Zero(), Cast<ArogueyPawn>(GetPawn()), Cast<ArogueyPawn>(InputActor));
		RogueyGameMode->InputManager->EnqueueInput(Input);
	}
	if (InteractType == EInteractType::TAKE)
	{
		const FInput Input(RogueyGameMode->GetCurrentTick(), EInputType::PICK_UP_ITEM, Cast<ArogueyPawn>(GetPawn()), Cast<ArogueyItemActor>(InputActor));
		RogueyGameMode->InputManager->EnqueueInput(Input);
	}
}

void ArogueyPlayerController::InventoryItemHoverAction(int32 InventoryIndex)
{
	FrogueyItem Item = RogueyGameMode->InventoryManager->GetItemAt(InventoryIndex);
	if (Item.ItemId == -1) return;
	OnHoverEvent.Broadcast(EInventoryEventTypeToString(Item.Interacts[0]), "<Yellow>"+ Item.ItemName + "</>");
}

void ArogueyPlayerController::InventoryItemInput(int32 InventoryIndex)
{
	FrogueyItem Item = RogueyGameMode->InventoryManager->GetItemAt(InventoryIndex);
	if (Item.ItemId == -1) return;

	FInventoryEvent InventoryEvent;
	InventoryEvent.EventType = Item.Interacts[0];
	InventoryEvent.FromIndex = InventoryIndex;
	
	RogueyGameMode->InventoryManager->EnqueueIventoryEvent(InventoryEvent);
}

void ArogueyPlayerController::InventoryItemInputWithEventType(int32 InventoryIndex, EInventoryEventType InventoryEventType)
{
	FrogueyItem Item = RogueyGameMode->InventoryManager->GetItemAt(InventoryIndex);
	if (Item.ItemId == -1) return;

	FInventoryEvent InventoryEvent;
	InventoryEvent.EventType = InventoryEventType;
	InventoryEvent.FromIndex = InventoryIndex;
	
	RogueyGameMode->InventoryManager->EnqueueIventoryEvent(InventoryEvent);
}

void ArogueyPlayerController::InventoryItemMenuInput(int32 InventoryIndex)
{
	if (!RogueyGameMode->InventoryManager->Inventory.Items.Contains(InventoryIndex))
	{
		return;
	}
	bIsInteractMenuOpen = true;
	FrogueyItem Item = RogueyGameMode->InventoryManager->GetItemAt(InventoryIndex);
	InteractMenuEntries.Empty();
	FInteractTypeArray InteractActorArray;
	InteractActorArray.InventoryEvents = Item.Interacts;
	InteractActorArray.Name = Item.ItemName;
	InteractActorArray.InventoryIndex = InventoryIndex;
	InteractMenuEntries.Add(InteractActorArray);
	OnInteractInventoryMenuEvent.Broadcast();
}

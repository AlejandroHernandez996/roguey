// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
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
	RogueyGameMode->GridManager->AddActorToGrid(GetCharacter(),GridUtils::WorldToGrid(GetCharacter()->GetActorLocation()));
	RogueyGameMode->GridManager->MoveActorInGrid(GetCharacter(),GridUtils::WorldToGrid(GetCharacter()->GetActorLocation()));
	if (GetCharacter())
	{
		CameraBoom = GetCharacter()->FindComponentByClass<USpringArmComponent>();
		FollowCamera = GetCharacter()->FindComponentByClass<UCameraComponent>();
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
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ArogueyPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ArogueyPlayerController::OnInputTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ArogueyPlayerController::OnInputReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ArogueyPlayerController::OnInputReleased);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ArogueyPlayerController::OnZoomTriggered);
		EnhancedInputComponent->BindAction(MouseScrollClickAction, ETriggerEvent::Started, this, &ArogueyPlayerController::OnMouseScrollStarted);
		EnhancedInputComponent->BindAction(MouseScrollClickAction, ETriggerEvent::Completed, this, &ArogueyPlayerController::OnMouseScrollReleased);
		EnhancedInputComponent->BindAction(MouseScrollClickAction, ETriggerEvent::Canceled, this, &ArogueyPlayerController::OnMouseScrollReleased);
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &ArogueyPlayerController::OnMouseMove);
	}
}

void ArogueyPlayerController::OnInputStarted()
{
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, true, Hit) && RogueyGameMode)
	{
		FInput Input(RogueyGameMode->GetCurrentTick(),EInputType::MOVEMENT_INPUT, Hit.Location, GetCharacter());
		RogueyGameMode->InputManager->EnqueueInput(Input);
		OnClickEvent.Broadcast();
	}
}

void ArogueyPlayerController::OnInputTriggered()
{
	
	
}

void ArogueyPlayerController::OnInputReleased()
{
}

void ArogueyPlayerController::OnZoomTriggered(const FInputActionInstance& Instance)
{
	float ZoomValue = -1.0f * Instance.GetValue().Get<float>();

	CameraBoom->TargetArmLength = FMath::Clamp(
		CameraBoom->TargetArmLength + ZoomValue * 50.0f,
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
	float RotationSpeed = -3.0f;
	float MinPitch = -80.0f;
	float MaxPitch = -15.0f;
	
	if (bIsRotating && CameraBoom)
	{
		FVector2D MoveValue = Instance.GetValue().Get<FVector2D>();
		UE_LOG(LogTemp, Log, TEXT("Mouse X: %f, Mouse Y: %f"), MoveValue.X, MoveValue.Y);
		FRotator NewRotation = CameraBoom->GetComponentRotation();
		NewRotation.Yaw += MoveValue.X * RotationSpeed;
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - MoveValue.Y * RotationSpeed, MinPitch, MaxPitch);

		CameraBoom->SetWorldRotation(NewRotation);
	}
}

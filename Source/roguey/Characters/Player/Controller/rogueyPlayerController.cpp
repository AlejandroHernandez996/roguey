// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "rogueyGameMode.h"
#include "Engine/LocalPlayer.h"
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
	}
}

void ArogueyPlayerController::OnInputStarted()
{
}

void ArogueyPlayerController::OnInputTriggered()
{
	
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, true, Hit) && RogueyGameMode)
	{
		FInput Input(RogueyGameMode->GetCurrentTick(),EInputType::MOVEMENT_INPUT, FVector2d(Hit.Location.X, Hit.Location.Y), Cast<ArogueyActor>(GetCharacter()));
		RogueyGameMode->InputManager->EnqueueInput(Input);
	}
}

void ArogueyPlayerController::OnInputReleased()
{
}
// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyGameMode.h"

#include "Core/Engine/rogueyEngine.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Input/rogueyInputManager.h"

ArogueyGameMode::ArogueyGameMode()
{
	Engine = NewObject<UrogueyEngine>(this, UrogueyEngine::StaticClass(), "Roguey Engine");
	MovementManager = NewObject<UrogueyMovementManager>(this, UrogueyMovementManager::StaticClass(), "Movement Manager");
	GridManager = NewObject<UrogueyGridManager>(this, UrogueyGridManager::StaticClass(), "Grid Manager");
	InputManager = NewObject<UrogueyInputManager>(this, UrogueyInputManager::StaticClass(), "Input Manager");
}

void ArogueyGameMode::BeginPlay()
{
	Super::BeginPlay();
	InputManager->MovementManager = MovementManager;
	MovementManager->GridManager = GridManager;
	Engine->Init({InputManager, MovementManager, GridManager});
}

void ArogueyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Engine->Process(DeltaTime);
}

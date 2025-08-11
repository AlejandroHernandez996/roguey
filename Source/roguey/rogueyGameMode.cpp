// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyGameMode.h"

#include "Core/Engine/rogueyEngine.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Combat/rogueyCombatManager.h"
#include "Input/rogueyInputManager.h"
#include "Inventory/rogueyInventoryManager.h"
#include "Items/rogueyItemCache.h"

ArogueyGameMode::ArogueyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ArogueyGameMode::BeginPlay()
{
	Super::BeginPlay();

	Engine = NewObject<UrogueyEngine>(this, UrogueyEngine::StaticClass(), "Roguey Engine");
	MovementManager = NewObject<UrogueyMovementManager>(this, UrogueyMovementManager::StaticClass(), "Movement Manager");
	GridManager = NewObject<UrogueyGridManager>(this, UrogueyGridManager::StaticClass(), "Grid Manager");
	InputManager = NewObject<UrogueyInputManager>(this, UrogueyInputManager::StaticClass(), "Input Manager");
	CombatManager = NewObject<UrogueyCombatManager>(this, UrogueyCombatManager::StaticClass(), "Combat Manager");
	InventoryManager = NewObject<UrogueyInventoryManager>(this, UrogueyInventoryManager::StaticClass(), "Inventory Manager");

	ItemCache = NewObject<UrogueyItemCache>(this, UrogueyItemCache::StaticClass(), "Item Cache");
	
	InputManager->MovementManager = MovementManager;
	InputManager->CombatManager = CombatManager;
	MovementManager->GridManager = GridManager;
	MovementManager->CombatManager = CombatManager;
	CombatManager->GridManager = GridManager;
	GridManager->Init();
	Engine->Init({InputManager, InventoryManager, MovementManager, GridManager, CombatManager});

	ItemCache->LoadItems();
}

void ArogueyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Engine->Process(DeltaTime);
}

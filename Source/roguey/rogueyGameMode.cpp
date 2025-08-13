// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyGameMode.h"

#include "AI/DecisionMaking/rogueyBehaviorManager.h"
#include "Core/Engine/rogueyEngine.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Characters/SpawnManager.h"
#include "Combat/DeathManager.h"
#include "Combat/rogueyCombatManager.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
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
	BehaviorManager = NewObject<UrogueyBehaviorManager>(this, UrogueyBehaviorManager::StaticClass(), "Behavior Manager");
	DeathManager = NewObject<UDeathManager>(this, UDeathManager::StaticClass(), "Death Manager");
	SpawnManager = NewObject<USpawnManager>(this, USpawnManager::StaticClass(), "Spawn Manager");

	InputManager->MovementManager = MovementManager;
	InputManager->CombatManager = CombatManager;
	
	MovementManager->GridManager = GridManager;
	MovementManager->CombatManager = CombatManager;
	
	CombatManager->GridManager = GridManager;
	CombatManager->InputManager = InputManager;
	CombatManager->DeathManager = DeathManager;
	
	BehaviorManager->GridManager = GridManager;
	BehaviorManager->InputManager = InputManager;
	
	DeathManager->GridManager = GridManager;
	DeathManager->SpawnManager = SpawnManager;

	SpawnManager->ItemCache = ItemCache;
	
	GridManager->Init();
	Engine->Init({
		SpawnManager,
		BehaviorManager,
		InputManager,
		InventoryManager,
		MovementManager,
		GridManager,
		CombatManager,
		DeathManager,
		SpawnManager}
		);
}

void ArogueyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Engine->Process(DeltaTime);
}

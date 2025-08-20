// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyGameMode.h"

#include "AI/DecisionMaking/rogueyBehaviorManager.h"
#include "Core/Engine/rogueyEngine.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Characters/rogueyNpcSpawner.h"
#include "Characters/SpawnManager.h"
#include "Combat/DeathManager.h"
#include "Combat/rogueyCombatManager.h"
#include "Combat/Projectile/RogueyProjectileManager.h"
#include "Grid/rogueyGridManager.h"
#include "Input/rogueyInputManager.h"
#include "Interactions/RogueyInteractManager.h"
#include "Inventory/rogueyInventoryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/URogueyResourceManager.h"

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
	InteractManager = NewObject<URogueyInteractManager>(this, URogueyInteractManager::StaticClass(), "Interact Manager");
	ResourceManager = NewObject<UURogueyResourceManager>(this, UURogueyResourceManager::StaticClass(), "Resource Manager");
	ProjectileManager = NewObject<URogueyProjectileManager>(this, URogueyProjectileManager::StaticClass(), "Projectile Manager");
	
	InputManager->MovementManager = MovementManager;
	InputManager->CombatManager = CombatManager;
	InputManager->InteractManager = InteractManager;
	
	MovementManager->GridManager = GridManager;
	MovementManager->CombatManager = CombatManager;
	MovementManager->InventoryManager = InventoryManager;
	MovementManager->InteractManager = InteractManager;
	
	CombatManager->GridManager = GridManager;
	CombatManager->InputManager = InputManager;
	CombatManager->DeathManager = DeathManager;
	CombatManager->ProjectileManager = ProjectileManager;
	
	BehaviorManager->GridManager = GridManager;
	BehaviorManager->InputManager = InputManager;
	
	DeathManager->GridManager = GridManager;
	DeathManager->SpawnManager = SpawnManager;

	SpawnManager->ItemCache = ItemCache;
	SpawnManager->GridManager = GridManager;

	InventoryManager->SpawnManager = SpawnManager;
	InventoryManager->GridManager = GridManager;

	ResourceManager->GridManager = GridManager;

	ProjectileManager->GridManager = GridManager;

	InteractManager->Init(ItemCache);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ArogueyNpcSpawner::StaticClass(), FoundActors);
	TSet<ArogueyNpcSpawner*> SpawnerSet;
	for (AActor* Actor : FoundActors)
	{
		if (ArogueyNpcSpawner* Spawner = Cast<ArogueyNpcSpawner>(Actor))
		{
			SpawnerSet.Add(Spawner);
		}
	}
	SpawnManager->NpcSpawners = SpawnerSet;

	TArray<AActor*> FoundObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArogueyObject::StaticClass(), FoundObjects);
	TSet<AArogueyObject*> ObjectSet;
	for (AActor* Actor : FoundObjects)
	{
		if (AArogueyObject* Object = Cast<AArogueyObject>(Actor))
		{
			ObjectSet.Add(Object);
		}
	}
	GridManager->Init(ObjectSet);

	Engine->Init({
		BehaviorManager,
		InputManager,
		MovementManager,
		InventoryManager,
		GridManager,
		InteractManager,
		ResourceManager,
		CombatManager,
		ProjectileManager,
		DeathManager,
		SpawnManager,
	});
}

void ArogueyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Engine->Process(DeltaTime);
}

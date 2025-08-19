// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Engine/Tickable.h"
#include "Enemies/rogueyLoot.h"
#include "Items/rogueyItem.h"
#include "UObject/Object.h"
#include "SpawnManager.generated.h"

class ArogueyNpcSpawner;
class UrogueyItemCache;
class ArogueyPawn;
class UrogueyGridManager;
/**
 * 
 */
UCLASS()
class ROGUEY_API USpawnManager : public UObject, public ITickable
{
	GENERATED_BODY()
public:
	
	void RogueyTick(int32 TickIndex) override;
	void EnqueueItem(const FrogueyItem Item);
	void EnqueueLootItem(const FrogueyLoot Item);
	void EnqueuePawn(TSubclassOf<ArogueyPawn> Pawn);
	void InitLootTable(ArogueyPawn* Pawn);
	void ProcessItemSpawns();
	void ProcessNpcSpawners(int32 TickIndex);
	
	TQueue<FrogueyItem> ItemSpawnQueue;
	TQueue<TSubclassOf<ArogueyPawn>> PawnSpawnQueue;
	UPROPERTY()
	UrogueyItemCache* ItemCache;
	UPROPERTY()
	UrogueyGridManager* GridManager;
	UPROPERTY()
	TSet<ArogueyNpcSpawner*> NpcSpawners;
};

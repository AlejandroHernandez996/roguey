// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathManager.h"

#include "Characters/rogueyPawn.h"
#include "Characters/SpawnManager.h"
#include "Grid/rogueyGridManager.h"

void UDeathManager::RogueyTick(int32 TickIndex)
{
	while (!DeathQueue.IsEmpty())
	{
		ArogueyPawn* DeadPawn;
		DeathQueue.Dequeue(DeadPawn);

		FrogueyItem LootItem = DeadPawn->LootTable.RollLoot();
		LootItem.SpawnGridPosition = GridManager->Grid.ActorMapLocation[DeadPawn];
		SpawnManager->EnqueueItem(LootItem);
		GridManager->RemoveActorFromGrid(DeadPawn);
		DeadPawn->Destroy();
	}
}

void UDeathManager::EnqueueDeath(ArogueyPawn* DeadPawn)
{
	DeathQueue.Enqueue(DeadPawn);
}

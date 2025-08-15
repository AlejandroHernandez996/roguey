// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathManager.h"

#include "Characters/rogueyPawn.h"
#include "Characters/SpawnManager.h"
#include "Grid/rogueyGridManager.h"

void UDeathManager::RogueyTick(int32 TickIndex)
{
	for (AActor* ActorToDestroy : PawnsToDestroy)
	{
		if (ActorToDestroy)
		{
			ActorToDestroy->Destroy();
		}
	}
	while (!DeathQueue.IsEmpty())
	{
		ArogueyPawn* DeadPawn;
		DeathQueue.Dequeue(DeadPawn);

		FrogueyItem LootItem = DeadPawn->LootTable.RollLoot();
		LootItem.SpawnGridPosition = GridManager->Grid.ActorMapLocation[DeadPawn];
		SpawnManager->EnqueueItem(LootItem);
		GridManager->RemoveActorFromGrid(DeadPawn);
		PawnsToDestroy.Add(DeadPawn);
		DeadPawn->SetPawnState(EPawnState::DEAD, true);
		for (ArogueyPawn* Threats : DeadPawn->ThreatList)
		{
			if (Threats->TargetPawn == DeadPawn)
			{
				Threats->TargetPawn = nullptr;
			}
		}
		DeadPawn->SetHidden(true);
	}
}

void UDeathManager::EnqueueDeath(ArogueyPawn* DeadPawn)
{
	DeathQueue.Enqueue(DeadPawn);
}

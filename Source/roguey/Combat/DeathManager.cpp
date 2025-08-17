// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathManager.h"

#include "Characters/rogueyPawn.h"
#include "Characters/SpawnManager.h"
#include "Grid/rogueyGridManager.h"

void UDeathManager::RogueyTick(int32 TickIndex)
{
	for (TWeakObjectPtr<ArogueyPawn> ActorToDestroy : PawnsToDestroy)
	{
		if (ActorToDestroy.Get())
		{
			GridManager->RemoveActorFromGrid(ActorToDestroy.Get());
			for (TWeakObjectPtr<ArogueyPawn> Threats : ActorToDestroy->ThreatList)
			{
				if (Threats->TargetPawn == ActorToDestroy)
				{
					Threats->TargetPawn = nullptr;
				}
			}
			ActorToDestroy->Destroy();
		}
	}
	while (!DeathQueue.IsEmpty())
	{
		TWeakObjectPtr<ArogueyPawn> DeadPawn;
		DeathQueue.Dequeue(DeadPawn);
		if (!DeadPawn.IsValid() || DeadPawn->PawnState == EPawnState::DEAD)
		{
			continue;
		}
		if (GridManager->Grid.ActorMapLocation.Contains(DeadPawn) )
		{
			FrogueyItem LootItem = DeadPawn->LootTable.RollLoot();
			LootItem.SpawnGridPosition = GridManager->Grid.ActorMapLocation[DeadPawn];
			SpawnManager->EnqueueItem(LootItem);
			PawnsToDestroy.Add(DeadPawn);
			DeadPawn->SetPawnState(EPawnState::DEAD, true);
		}
	}
}

void UDeathManager::EnqueueDeath(ArogueyPawn* DeadPawn)
{
	DeathQueue.Enqueue(DeadPawn);
}

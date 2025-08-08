// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyMovementManager.h"
#include "rogueyPathfinder.h"

#include "Path.h"
#include "Grid/rogueyGridManager.h"

void UrogueyMovementManager::RogueyTick(uint32 TickIndex)
{
	while (MovementQueue.IsEmpty())
	{
		FMovement ProcessMovement;
		MovementQueue.Dequeue(ProcessMovement);
		if (ProcessMovement.Actor != nullptr)
		{
			FPath NewPath = UrogueyPathfinder::FindAndGeneratePath(ProcessMovement, GridManager->Grid);
			if (ActivePaths.Contains(ProcessMovement.Actor))
			{
				ActivePaths.Remove(ProcessMovement.Actor);
				ActivePaths.Add(ProcessMovement.Actor, NewPath);
			}
		}
	}

	TSet<ArogueyActor*> FinishedAPathActors;
	for (auto& ActorAndPath : ActivePaths)
	{
		ArogueyActor* PathActor = ActorAndPath.Key;
		FPath Path = ActorAndPath.Value;

		FGridEvent MoveEvent = FGridEvent(TickIndex, Path.GetMovementLocation(), PathActor, EGridEventType::MOVE);
		GridManager->EnqueueGridEvent(MoveEvent);
		Path.PathIndex += 1;
		if (Path.IsPathComplete())
		{
			FinishedAPathActors.Add(PathActor);
		}
	}
	for (auto& FinishedActor : FinishedAPathActors)
	{
		ActivePaths.Remove(FinishedActor);
	}
}

void UrogueyMovementManager::EnqueueMovement(const FMovement& Movement)
{
	MovementQueue.Enqueue(Movement);
}

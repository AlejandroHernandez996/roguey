// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyMovementManager.h"

#include "ActorPath.h"
#include "rogueyPathfinder.h"

#include "Path.h"
#include "Grid/rogueyGridManager.h"
#include "Grid/Util/GridUtils.h"

void UrogueyMovementManager::RogueyTick(uint32 TickIndex)
{
	while (!MovementQueue.IsEmpty())
	{
		FMovement ProcessMovement;
		MovementQueue.Dequeue(ProcessMovement);
		UE_LOG(LogTemp, Log, TEXT("Processing Movement - Tick: %u,Destination: %s, Actor: %s"),
			  ProcessMovement.Tick,
			  *ProcessMovement.Destination.ToString(),
			  ProcessMovement.Actor ? *ProcessMovement.Actor->GetName() : TEXT("None")
		  );
		if (ProcessMovement.Actor != nullptr)
		{
			FPath NewPath = UrogueyPathfinder::FindAndGeneratePath(ProcessMovement, GridManager->Grid);
			if (ActivePaths.Contains(ProcessMovement.Actor))
			{
				ActivePaths.Remove(ProcessMovement.Actor);
			}
			ActivePaths.Add(ProcessMovement.Actor, NewPath);
			
			FActorPath ActorPath;
			ActorPath.MovementPath = NewPath.MovementPath;
			ActorPaths.Add(ProcessMovement.Actor, ActorPath);

			FString PathString = FString::Printf(TEXT("Actor %p Path: "), ProcessMovement.Actor);

			for (const FIntVector2& Point : NewPath.MovementPath)
			{
				PathString += FString::Printf(TEXT("(%d, %d) -> "), Point.X, Point.Y);
			}
			PathString.RemoveFromEnd(TEXT(" -> ")); // Remove the last arrow

			UE_LOG(LogTemp, Log, TEXT("%s"), *PathString);
		}
	}

	TSet<AActor*> FinishedAPathActors;
	for (auto& ActorAndPath : ActivePaths)
	{
		AActor* PathActor = ActorAndPath.Key;
		FPath& Path = ActivePaths[ActorAndPath.Key];
		FGridEvent GridEvent = FGridEvent(TickIndex, Path.GetMovementLocation(), PathActor, EGridEventType::MOVE);
		GridManager->EnqueueGridEvent(GridEvent);
		UE_LOG(LogTemp, Log, TEXT("GridEvent Enqueued -- Tick: %u, EventType: %s, Actor: %s, Location: (%d, %d)"),
			GridEvent.Tick,
			*UEnum::GetValueAsString(GridEvent.EventType),
			GridEvent.Actor ? *GridEvent.Actor->GetName() : TEXT("None"),
			GridEvent.Location.X,
			GridEvent.Location.Y
		);
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

	UE_LOG(LogTemp, Log, TEXT("Movement Enqueued - Tick: %u,Destination: %s, Actor: %s"),
			  Movement.Tick,
			  *Movement.Destination.ToString(),
			  Movement.Actor ? *Movement.Actor->GetName() : TEXT("None")
		  );
	MovementQueue.Enqueue(Movement);
}

void UrogueyMovementManager::Tick(float DeltaTime)
{
	
}

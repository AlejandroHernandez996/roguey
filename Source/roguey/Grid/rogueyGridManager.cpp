// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyGridManager.h"

#include "Util/GridUtils.h"

void UrogueyGridManager::RogueyTick(uint32 TickIndex)
{
	while (!GridQueue.IsEmpty())
	{
		FGridEvent GridEvent;
		GridQueue.Dequeue(GridEvent);

		switch (GridEvent.EventType)
		{
		case EGridEventType::MOVE:
			MoveActorInGrid(GridEvent.Actor, GridEvent.Location);
			break;
		case EGridEventType::ADD:
			AddActorToGrid(GridEvent.Actor, GridEvent.Location);
			break;
		case EGridEventType::REMOVE:
			RemoveActorFromGrid(GridEvent.Actor);
			break;
		case EGridEventType::NONE:
		default: ;
		}
	}
}

void UrogueyGridManager::AddActorToGrid(ArogueyActor* Actor, FIntVector2 Location)
{
	if (Actor)
	{
		ActorMapLocation.Add(Actor, Location);
		if (!Grid.GridMap.Contains(Location))
		{
			Grid.GridMap.Add(Location, FTile());
		}
		Grid.GridMap[Location].ActorsInTile.Add(Actor);
	}
}

void UrogueyGridManager::RemoveActorFromGrid(ArogueyActor* Actor)
{
	if (Actor && ActorMapLocation.Contains(Actor))
	{
		FIntVector2 Location = ActorMapLocation[Actor];
		ActorMapLocation.Remove(Actor);
		if (Grid.GridMap.Contains(Location))
		{
			Grid.GridMap[Location].ActorsInTile.Remove(Actor);
		}
	}
}

void UrogueyGridManager::MoveActorInGrid(ArogueyActor* Actor, FIntVector2 Destination)
{
	RemoveActorFromGrid(Actor);
	AddActorToGrid(Actor, Destination);

	Actor->SetActorLocation(FVector(GridUtils::GridToWorld(Destination)));
}

void UrogueyGridManager::EnqueueGridEvent(const FGridEvent& GridEvent)
{
	GridQueue.Enqueue(GridEvent);
}

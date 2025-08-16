// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyGridManager.h"
#include "DrawDebugHelpers.h"

void UrogueyGridManager::RogueyTick(int32 TickIndex)
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

void UrogueyGridManager::AddActorToGrid(TWeakObjectPtr<ArogueyPawn> Actor, FIntVector2 Location)
{
	if (Actor.Get())
	{
		Grid.ActorMapLocation.Add(Actor, Location);
		if (!Grid.GridMap.Contains(Location))
		{
			Grid.GridMap.Add(Location, FTile());
		}
		Grid.GridMap[Location].ActorsInTile.Add(Actor);
	}
}

void UrogueyGridManager::RemoveActorFromGrid(TWeakObjectPtr<ArogueyPawn> Actor)
{
	if (Actor.Get() && Grid.ActorMapLocation.Contains(Actor))
	{
		FIntVector2 Location = Grid.ActorMapLocation[Actor];
		Grid.ActorMapLocation.Remove(Actor);
		if (Grid.GridMap.Contains(Location))
		{
			Grid.GridMap[Location].ActorsInTile.Remove(Actor);
		}
	}
}

void UrogueyGridManager::MoveActorInGrid(TWeakObjectPtr<ArogueyPawn> Actor, FIntVector2 Destination)
{
	FIntVector2 Start = Grid.ActorMapLocation[Actor];
	float Distance = FMath::Abs(FVector2d::Distance(FVector2d(Destination.X, Destination.Y), FVector2d(Start.X, Start.Y)));
	Actor->TrueTileQueue.Enqueue({Destination, Distance});
	Actor->QueueSize++;
	RemoveActorFromGrid(Actor);
	AddActorToGrid(Actor, Destination);
}

void UrogueyGridManager::EnqueueGridEvent(const FGridEvent& GridEvent)
{
	GridQueue.Enqueue(GridEvent);
}

FIntVector2 UrogueyGridManager::GetActorTrueTile(TWeakObjectPtr<ArogueyPawn> Actor)
{
	if (!Actor.Get() || !Grid.ActorMapLocation.Contains(Actor)) return FIntVector2::ZeroValue;

	return Grid.ActorMapLocation[Actor];
}

bool UrogueyGridManager::GridContainsActor(TWeakObjectPtr<ArogueyPawn> Actor)
{
	return Grid.ActorMapLocation.Contains(Actor);
}

void UrogueyGridManager::Init()
{
	for (int i = 0; i < GridSize.X ; i++)
	{
		for (int j = 0; j < GridSize.Y ; j++)
		{
			FTile Tile = FTile();
			Grid.GridMap.Add(FIntVector2(i, j), Tile);
		}
	}
}

FIntVector2 UrogueyGridManager::GetPlayerTrueLocation()
{
	return Grid.ActorMapLocation[PlayerCharacter];
}

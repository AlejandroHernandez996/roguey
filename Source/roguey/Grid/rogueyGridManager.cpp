// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyGridManager.h"

#include "Characters/Player/rogueyCharacter.h"
#include "DrawDebugHelpers.h"
#include "Util/GridUtils.h"

void UrogueyGridManager::RogueyTick(uint32 TickIndex)
{
	while (!GridQueue.IsEmpty())
	{
		FGridEvent GridEvent;
		GridQueue.Dequeue(GridEvent);

		UE_LOG(LogTemp, Log, TEXT("GridEvent Processing -- Tick: %u, EventType: %s, Actor: %s, Location: (%d, %d)"),
			GridEvent.Tick,
			*UEnum::GetValueAsString(GridEvent.EventType),
			GridEvent.Actor ? *GridEvent.Actor->GetName() : TEXT("None"),
			GridEvent.Location.X,
			GridEvent.Location.Y
		);
		
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

void UrogueyGridManager::AddActorToGrid(AActor* Actor, FIntVector2 Location)
{
	if (Actor)
	{
		Grid.ActorMapLocation.Add(Actor, Location);
		if (!Grid.GridMap.Contains(Location))
		{
			Grid.GridMap.Add(Location, FTile());
		}
		Grid.GridMap[Location].ActorsInTile.Add(Actor);
	}
}

void UrogueyGridManager::RemoveActorFromGrid(AActor* Actor)
{
	if (Actor && Grid.ActorMapLocation.Contains(Actor))
	{
		FIntVector2 Location = Grid.ActorMapLocation[Actor];
		Grid.ActorMapLocation.Remove(Actor);
		if (Grid.GridMap.Contains(Location))
		{
			Grid.GridMap[Location].ActorsInTile.Remove(Actor);
		}
	}
}

void UrogueyGridManager::MoveActorInGrid(AActor* Actor, FIntVector2 Destination)
{
	UE_LOG(LogTemp, Log, TEXT("Moving Actor in Grid"));
	FIntVector2 Start = Grid.ActorMapLocation[Actor];
	float Distance = FMath::Abs(FVector2d::Distance(FVector2d(Destination.X, Destination.Y), FVector2d(Start.X, Start.Y)));
	float SpeedMultiplier = Distance >=1.8f ? 2.0f : 1.0f;
	UE_LOG(LogTemp, Log, TEXT("Distance: %f") ,Distance);
	if (ArogueyCharacter* RougeyChar = Cast<ArogueyCharacter>(Actor))
	{
		RougeyChar->TrueTileQueue.Enqueue({Destination, Distance});
		RougeyChar->QueueSize++;
	}
	RemoveActorFromGrid(Actor);
	AddActorToGrid(Actor, Destination);
	
}

void UrogueyGridManager::EnqueueGridEvent(const FGridEvent& GridEvent)
{
	GridQueue.Enqueue(GridEvent);
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
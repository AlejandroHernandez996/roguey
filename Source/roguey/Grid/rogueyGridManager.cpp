// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyGridManager.h"
#include "DrawDebugHelpers.h"
#include "rogueyGameMode.h"
#include "Inventory/rogueyInventoryManager.h"

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

void UrogueyGridManager::Init(TSet<AArogueyObject*> RogueyObjects)
{
	for (int i = 0; i < GridSize.X ; i++)
	{
		for (int j = 0; j < GridSize.Y ; j++)
		{
			FTile Tile = FTile();
			Grid.GridMap.Add(FIntVector2(i, j), Tile);
		}
	}
	
	for (auto& RogueyObject : RogueyObjects)
	{
		AddRogueyObjectToGrid(RogueyObject);
	}
}

FIntVector2 UrogueyGridManager::FindRandomTileInRangeOfPawn(TWeakObjectPtr<ArogueyPawn> Pawn, int Range)
{
    if (!Pawn.IsValid() || !Grid.ActorMapLocation.Contains(Pawn))
    {
        return FIntVector2(); 
    }

    const FIntVector2 Origin = Grid.ActorMapLocation[Pawn];

    if (Range <= 0)
    {
        return Origin;
    }

    TSet<FIntVector2> Occupied;
    Occupied.Reserve(Grid.ActorMapLocation.Num());
    for (const TPair<TWeakObjectPtr<ArogueyPawn>, FIntVector2>& Pair : Grid.ActorMapLocation)
    {
        Occupied.Add(Pair.Value);
    }

    TArray<FIntVector2> Candidates;
    Candidates.Reserve((Range * 2 + 1) * (Range * 2 + 1));

    const int32 MinX = Origin.X - Range;
    const int32 MaxX = Origin.X + Range;
    const int32 MinY = Origin.Y - Range;
    const int32 MaxY = Origin.Y + Range;

    for (int32 X = MinX; X <= MaxX; ++X)
    {
        for (int32 Y = MinY; Y <= MaxY; ++Y)
        {
            const FIntVector2 P(X, Y);

            if (P == Origin)
            {
                continue;
            }

            if (GridUtils::GridDistance(Origin, P) > Range)
            {
                continue;
            }
            if (Occupied.Contains(P))
            {
                continue;
            }

            Candidates.Add(P);
        }
    }

    if (Candidates.Num() == 0)
    {
        return Origin;
    }

    const int32 Index = FMath::RandHelper(Candidates.Num());
    return Candidates[Index];
}

void UrogueyGridManager::AddRogueyObjectToGrid(AArogueyObject* RogueyObject)
{
	if (RogueyObject)
	{
		FIntVector2 Location = GridUtils::WorldToGrid(RogueyObject->GetActorLocation());
		Grid.ObjectMapLocation.Add(RogueyObject, Location);
		Grid.GridMap[Location].TileType = ETileType::BLOCKED;
	}
}

FIntVector2 UrogueyGridManager::GetPlayerTrueLocation()
{
	return Grid.ActorMapLocation[PlayerCharacter];
}

bool UrogueyGridManager::IsPawnInRange(TWeakObjectPtr<ArogueyPawn> From, TWeakObjectPtr<ArogueyPawn> To)
{
	{
		if (!Grid.ActorMapLocation.Contains(From) || !Grid.ActorMapLocation.Contains(To))
		{
			return false;
		}
		FIntVector2 FromPoint = Grid.ActorMapLocation[From];
		FIntVector2 ToPoint = Grid.ActorMapLocation[To];
		int32 CurrentRange = From->StatPage.StatPage[ErogueyStatType::ATTACK_RANGE].CurrentLevel;
		CurrentRange = FMath::Max(CurrentRange, Cast<ArogueyGameMode>(From->GetWorld()->GetAuthGameMode())->InventoryManager->GetTotalBonusByStat(EItemStatType::ATTACK_RANGE));
		int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
		if (CurrentRange == 1)
		{
			return GridUtils::IsAdjacent(FromPoint, ToPoint);
		}
		return FromPoint != ToPoint && CurrentRange >= Distance;
	}
}


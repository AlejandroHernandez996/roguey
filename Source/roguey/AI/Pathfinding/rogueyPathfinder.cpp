#include "rogueyPathfinder.h"

#include "Movement.h"
#include "Path.h"
#include "Grid/Grid.h"
#include "Engine/Engine.h" // Include for logging

FPath UrogueyPathfinder::FindAndGeneratePath(FMovement Movement, FGrid Grid)
{
    FPath Path;
    FIntVector2 Start = Grid.ActorMapLocation[Movement.Actor];
    FIntVector2 Destination = Movement.Destination;

    if (Start == Destination)
    {
        return Path;
    }

    static const TArray<FIntVector2> NeighborOrder = {
        { -1,  0 }, // West
        {  1,  0 }, // East
        {  0,  1 }, // South
        {  0, -1 }, // North
        { -1,  1 }, // SW
        {  1,  1 }, // SE
        { -1, -1 }, // NW
        {  1, -1 }  // NE
    };

    TQueue<FIntVector2> Open;
    TMap<FIntVector2, FIntVector2> CameFrom;
    TSet<FIntVector2> Visited;

    Open.Enqueue(Start);
    Visited.Add(Start);

    bool PathFound = false;
    while (!Open.IsEmpty())
    {
        FIntVector2 Current;
        Open.Dequeue(Current);

        if (Current == Destination)
        {
            PathFound = true;
            break;
        }

        for (const FIntVector2& Dir : NeighborOrder)
        {
            FIntVector2 Neighbor = Current + Dir;
            if (Visited.Contains(Neighbor))
                continue;
            if (!Grid.CanMove(Current, Neighbor))
                continue;

            Open.Enqueue(Neighbor);
            Visited.Add(Neighbor);
            CameFrom.Add(Neighbor, Current);
        }
    }

    if (!PathFound)
    {
        Path.PathIndex = -1;
        return Path; // No path
    }

    // Backtrack from destination to start to build path
    TArray<FIntVector2> ReversePath;
    FIntVector2 Step = Destination;
    while (Step != Start)
    {
        ReversePath.Add(Step);
        Step = CameFrom[Step];
    }
    ReversePath.Add(Start);
    Algo::Reverse(ReversePath);

    Path.MovementPath = ReversePath;
    Path.PathIndex = 1;

    // Log the entire path
    FString PathString = "Path: ";
    for (const FIntVector2& Point : ReversePath)
    {
        PathString += FString::Printf(TEXT("(%d, %d) -> "), Point.X, Point.Y);
    }
    PathString.RemoveFromEnd(TEXT(" -> ")); // Remove the last arrow
    UE_LOG(LogTemp, Log, TEXT("%s"), *PathString);

    return Path;
}
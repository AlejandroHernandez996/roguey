#include "rogueyPathfinder.h"

#include "Movement.h"
#include "Path.h"
#include "Grid/Grid.h"
#include "Engine/Engine.h" // Include for logging
#include "GeometryCollection/Facades/CollectionConstraintOverrideFacade.h"
#include "Grid/Util/GridUtils.h"

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
        return Path;
    }

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
    Path.PathIndex = 0;

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

FPath UrogueyPathfinder::FindAndGeneratePathToPawn(FMovement Movement, FGrid Grid)
{
    FPath Path;
    ArogueyPawn* TargetPawn = Movement.TargetPawn;
    if (TargetPawn == nullptr)
    {
        Path.PathIndex = -1;
        return Path;
    }

    FIntVector2 Start = Grid.ActorMapLocation[Movement.Actor];
    FIntVector2 TargetLocation = Grid.ActorMapLocation[TargetPawn];

    auto IsOrthogonallyAdjacent = [](const FIntVector2& A, const FIntVector2& B)
    {
        return (FMath::Abs(A.X - B.X) == 1 && A.Y == B.Y) ||
               (FMath::Abs(A.Y - B.Y) == 1 && A.X == B.X);
    };

    if (IsOrthogonallyAdjacent(Start, TargetLocation))
    {
        Path.PathIndex = 0;
        Path.TargetPawn = TargetPawn;
        Path.TargetPosition = TargetLocation;
        return Path;
    }

    if (Start == TargetLocation)
    {
        static const TArray<FIntVector2> OrthogonalDirs = {
            { -1,  0 }, { 1,  0 }, { 0,  1 }, { 0, -1 }
        };

        for (const FIntVector2& Dir : OrthogonalDirs)
        {
            FIntVector2 Candidate = Start + Dir;
            if (Grid.CanMove(Start, Candidate))
            {
                Path.MovementPath = { Start, Candidate };
                Path.PathIndex = 0;
                Path.TargetPawn = TargetPawn;
                Path.TargetPosition = TargetLocation;
                return Path;
            }
        }

        Path.PathIndex = 0;
        return Path;
    }

    static const TArray<FIntVector2> NeighborOrder = {
        { -1,  0 }, { 1,  0 }, { 0,  1 }, { 0, -1 },
        { -1,  1 }, { 1,  1 }, { -1, -1 }, { 1, -1 }
    };

    TQueue<FIntVector2> Open;
    TMap<FIntVector2, FIntVector2> CameFrom;
    TSet<FIntVector2> Visited;

    Open.Enqueue(Start);
    Visited.Add(Start);

    bool PathFound = false;
    FIntVector2 ClosestTile;

    while (!Open.IsEmpty())
    {
        FIntVector2 Current;
        Open.Dequeue(Current);

        if (IsOrthogonallyAdjacent(Current, TargetLocation))
        {
            ClosestTile = Current;
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
        return Path;
    }

    TArray<FIntVector2> ReversePath;
    FIntVector2 Step = ClosestTile;
    while (Step != Start)
    {
        ReversePath.Add(Step);
        Step = CameFrom[Step];
    }
    ReversePath.Add(Start);
    Algo::Reverse(ReversePath);

    Path.MovementPath = ReversePath;
    Path.PathIndex = 0;
    Path.TargetPawn = TargetPawn;
    Path.TargetPosition = TargetLocation;

    return Path;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyMovementManager.h"

#include "ActorPath.h"
#include "rogueyPathfinder.h"

#include "Path.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Combat/rogueyCombatManager.h"
#include "Grid/rogueyGridManager.h"
#include "Inventory/InventoryEvent.h"
#include "Inventory/InventoryEventType.h"
#include "Inventory/rogueyInventoryManager.h"

void UrogueyMovementManager::RogueyTick(int32 TickIndex)
{
    HandleActivePaths(TickIndex);
    ProcessMovementQueue(TickIndex);
    ProcessActivePaths(TickIndex);
}

void UrogueyMovementManager::HandleActivePaths(int32 TickIndex)
{
    TSet< TWeakObjectPtr<ArogueyPawn>> FinishedPathActors;

    for (auto& ActorAndPath : ActivePaths)
    {
        TWeakObjectPtr<ArogueyPawn> PathActor = ActorAndPath.Key;
        FPath& Path = ActorAndPath.Value;

        if (PathActor->PawnState == EPawnState::DEAD)
        {
            FinishedPathActors.Add(PathActor);
            continue;
        }
        if (Path.TargetPawn.Get() && Path.TargetPawn->PawnState == EPawnState::DEAD)
        {
            FinishedPathActors.Add(PathActor);
            continue;
        }
        if (Path.TargetPawn.Get() && Path.TargetPosition != GridManager->Grid.ActorMapLocation[Path.TargetPawn])
        {
            EnqueueMovement(FMovement(PathActor.Get(), Path.TargetPawn.Get(), FIntVector2::ZeroValue, TickIndex));
            FinishedPathActors.Add(PathActor);
        }
    }

    for (auto& FinishedActor : FinishedPathActors)
    {
        ActivePaths.Remove(FinishedActor);
    }
}

void UrogueyMovementManager::ProcessMovementQueue(int32 TickIndex)
{
    while (!MovementQueue.IsEmpty())
    {
        FMovement ProcessMovement;
        MovementQueue.Dequeue(ProcessMovement);

        if (!ProcessMovement.Actor.Get() || ProcessMovement.Actor->PawnState == EPawnState::DEAD)
        {
            continue; 
        }
        
        FPath NewPath = GenerateNewPath(ProcessMovement);
        bool bIsAttackPathAndInRange = ProcessMovement.TargetPawn.Get() && GridManager->IsPawnInRange(ProcessMovement.Actor.Get(), ProcessMovement.TargetPawn.Get());
        if (NewPath.IsPathComplete() || bIsAttackPathAndInRange)
        {
            if (ProcessMovement.TargetPawn.Get())
            {
                CombatManager->EnqueueCombatEvent(FCombatEvent(ProcessMovement.Actor.Get(), ProcessMovement.TargetPawn.Get(), TickIndex));
            }
            if (ProcessMovement.TargetItem.Get())
            {
                FInventoryEvent InventoryEvent;
                InventoryEvent.EventType = EInventoryEventType::PICK_UP;
                InventoryEvent.ItemActor = ProcessMovement.TargetItem;
                InventoryManager->EnqueueIventoryEvent(InventoryEvent);
            }
            continue; 
        }

        UpdateActivePath(ProcessMovement.Actor.Get(), NewPath);
    }
}

FPath UrogueyMovementManager::GenerateNewPath(const FMovement& Movement)
{
    if (Movement.Actor.Get() && Movement.TargetPawn.Get() && Movement.TargetPawn->PawnState != EPawnState::DEAD)
    {
        return UrogueyPathfinder::FindAndGeneratePathToPawn(Movement, GridManager->Grid);
    }
    if (Movement.Actor.Get() && Movement.TargetItem.Get())
    {
        return UrogueyPathfinder::FindAndGeneratePathToItem(Movement, GridManager->Grid);
    }
    return UrogueyPathfinder::FindAndGeneratePath(Movement, GridManager->Grid);
}

void UrogueyMovementManager::UpdateActivePath(TWeakObjectPtr<ArogueyPawn> Actor, const FPath& NewPath)
{
    ActivePaths.Remove(Actor);
    ActivePaths.Add(Actor, NewPath);
    
    FActorPath ActorPath;
    ActorPath.MovementPath = NewPath.MovementPath;
    ActorPaths.Add(Actor, ActorPath);
}


void UrogueyMovementManager::ProcessActivePaths(int32 TickIndex)
{
    TSet<TWeakObjectPtr<ArogueyPawn>> FinishedPathActors;
    
    for (auto& ActorAndPath : ActivePaths)
    {
        TWeakObjectPtr<ArogueyPawn> PathActor = ActorAndPath.Key;
        FPath& Path = ActorAndPath.Value;

        if ((PathActor.IsValid() && PathActor->PawnState == EPawnState::DEAD) || (PathActor->TargetPawn.IsValid() && PathActor->TargetPawn->PawnState == EPawnState::DEAD))
        {
            FinishedPathActors.Add(PathActor);
            continue;
        }

        if (Path.TargetPawn.IsValid() && Path.TargetPosition != GridManager->Grid.ActorMapLocation[Path.TargetPawn])
        {
            EnqueueMovement(FMovement(PathActor, Path.TargetPawn, FIntVector2::ZeroValue, TickIndex));
            FinishedPathActors.Add(PathActor);
            continue;
        }

        if (Path.TargetPawn.IsValid() && Cast<ArogueyCharacter>(Path.TargetPawn.Get()))
        {
            FIntVector2 CurrentLocation = GridManager->Grid.ActorMapLocation[PathActor];
            FIntVector2 TargetActorLocation = GridManager->Grid.ActorMapLocation[Path.TargetPawn];

            if (Path.TargetPawn->TargetPawn == PathActor && CurrentLocation == TargetActorLocation)
            {
                continue;
            }
        }

        FIntVector2 NextPoint = Path.GetAndIncrementPath(PathActor->TileMoveSpeed);
        FGridEvent GridEvent = FGridEvent(TickIndex, NextPoint, PathActor, EGridEventType::MOVE);
        GridManager->EnqueueGridEvent(GridEvent);
        
        if (!ActorPaths.Contains(PathActor))
        {
            ActorPaths.Add(PathActor, FActorPath());
        }
        ActorPaths[PathActor].MovementPath.Add(NextPoint);

        if (Path.IsPathComplete() || (Path.TargetPawn.IsValid() && GridManager->IsPawnInRangeOfPoint(PathActor, NextPoint, Path.TargetPawn)))
        {
            FinishedPathActors.Add(PathActor);

            if (Path.TargetPawn.IsValid() && Path.TargetPawn->PawnState != EPawnState::DEAD)
            {
                CombatManager->EnqueueCombatEvent(FCombatEvent(PathActor, Path.TargetPawn, TickIndex));
            }
            if (Path.TargetItem.IsValid())
            {
                FInventoryEvent InventoryEvent;
                InventoryEvent.EventType = EInventoryEventType::PICK_UP;
                InventoryEvent.ItemActor = Path.TargetItem;
                InventoryManager->EnqueueIventoryEvent(InventoryEvent);
            }
        }
    }
    
    for (auto& FinishedActor : FinishedPathActors)
    {
        ActivePaths.Remove(FinishedActor);
    }
}

void UrogueyMovementManager::EnqueueMovement(const FMovement& Movement)
{
	MovementQueue.Enqueue(Movement);
}

void UrogueyMovementManager::RemoveActorFromActiveQueue(TWeakObjectPtr<ArogueyPawn> Pawn)
{
	ActivePaths.Remove(Pawn);
}

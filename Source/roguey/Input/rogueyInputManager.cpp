// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyInputManager.h"

#include "Input.h"
#include "AI/Pathfinding/Movement.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Combat/rogueyCombatManager.h"
#include "Grid/Util/GridUtils.h"
#include "Interactions/RogueyInteractManager.h"

void UrogueyInputManager::RogueyTick(int32 TickIndex)
{
	while (!InputQueue.IsEmpty())
	{
		FInput ProcessInput;
		InputQueue.Dequeue(ProcessInput);
		if (!ProcessInput.InputActor.IsValid() || ProcessInput.InputActor->PawnState == EPawnState::DEAD)
		{
			continue;
		}
		MovementManager->RemoveActorFromActiveQueue(ProcessInput.InputActor);
		CombatManager->RemoveActorFromActiveQueue(ProcessInput.InputActor);
		InteractManager->RemoveActorFromActive(ProcessInput.InputActor);
		FMovement Movement = FMovement(ProcessInput.InputActor, GridUtils::WorldToGrid(ProcessInput.InputWorldLocation), ProcessInput.InputTick);
		switch (ProcessInput.InputType)
		{
		case EInputType::MOVE:
			MovementManager->EnqueueMovement(Movement);
			ProcessInput.InputActor->ClearTarget();
			break;
		case EInputType::ATTACK:
			if (ProcessInput.TargetPawn.IsValid() && ProcessInput.TargetPawn->PawnState != EPawnState::DEAD)
			{
				Movement.TargetPawn = ProcessInput.TargetPawn;
				MovementManager->EnqueueMovement(Movement);
				ProcessInput.InputActor->SetTarget(ProcessInput.TargetPawn);
			}
			break;
		case EInputType::PICK_UP_ITEM:
			if (ProcessInput.TargetItem.Get())
			{
				Movement.TargetItem = ProcessInput.TargetItem;
				MovementManager->EnqueueMovement(Movement);
			}
			break;
		case EInputType::INTERACT:
			if (ProcessInput.TargetObject.Get())
			{
				Movement.TargetObject = ProcessInput.TargetObject;
				MovementManager->EnqueueMovement(Movement);
			}
			break;
		case EInputType::NONE:
			break;
		default: ;
		}
	}
}

void UrogueyInputManager::EnqueueInput(const FInput& Input)
{
	if (Input.InputType == EInputType::NONE) return;
	InputQueue.Enqueue(Input);
}

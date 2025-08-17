// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyInputManager.h"

#include "Input.h"
#include "AI/Pathfinding/Movement.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Combat/rogueyCombatManager.h"
#include "Grid/Util/GridUtils.h"

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
		switch (ProcessInput.InputType)
		{
		case EInputType::MOVE:
			MovementManager->EnqueueMovement(FMovement(ProcessInput.InputActor,nullptr, GridUtils::WorldToGrid(ProcessInput.InputWorldLocation),ProcessInput.InputTick));
			ProcessInput.InputActor->ClearTarget();
			break;
		case EInputType::FOLLOW:
			break;
		case EInputType::ATTACK:
			if (ProcessInput.TargetPawn.IsValid() && ProcessInput.TargetPawn->PawnState != EPawnState::DEAD)
			{
				MovementManager->EnqueueMovement(FMovement(ProcessInput.InputActor, ProcessInput.TargetPawn, FIntVector2::ZeroValue, ProcessInput.InputTick));
				ProcessInput.InputActor->SetTarget(ProcessInput.TargetPawn);
			}
			break;
		case EInputType::EQUIP:
			break;
		case EInputType::ABILITY:
			break;
		case EInputType::PICK_UP_ITEM:
			if (ProcessInput.TargetItem.Get())
			{
				MovementManager->EnqueueMovement(FMovement(ProcessInput.InputActor, ProcessInput.TargetItem, ProcessInput.InputTick));
			}
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

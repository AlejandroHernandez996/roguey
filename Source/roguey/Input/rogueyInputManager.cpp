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
		switch (ProcessInput.InputType)
		{
		case EInputType::MOVE:
			MovementManager->EnqueueMovement(FMovement(ProcessInput.InputActor,GridUtils::WorldToGrid(ProcessInput.InputWorldLocation),ProcessInput.InputTick));
			break;
		case EInputType::ATTACK:
			CombatManager->EnqueueCombatEvent(FCombatEvent(ProcessInput.InputActor, ProcessInput.TargetPawn, ProcessInput.InputTick));
			break;
		case EInputType::EQUIP:
			break;
		case EInputType::ABILITY:
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

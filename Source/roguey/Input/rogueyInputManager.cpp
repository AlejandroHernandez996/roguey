// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyInputManager.h"

#include "Input.h"
#include "AI/Pathfinding/Movement.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Grid/Util/GridUtils.h"

void UrogueyInputManager::RogueyTick(uint32 TickIndex)
{
	while (InputQueue.IsEmpty() || InputQueue.Peek()->InputTick > TickIndex)
	{
		FInput ProcessInput;
		InputQueue.Dequeue(ProcessInput);

		switch (ProcessInput.InputType)
		{
		case EInputType::MOVEMENT_INPUT:
			MovementManager->EnqueueMovement(FMovement(ProcessInput.InputActor,GridUtils::WorldToGrid(ProcessInput.InputWorldLocation),ProcessInput.InputTick));
			break;
		case EInputType::ATTACK_INPUT:
			break;
		case EInputType::ITEM_INPUT:
			break;
		case EInputType::ABILTY_INPUT:
			break;
		case EInputType::PRAYER_INPUT:
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

// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyMovementManager.h"
#include "rogueyPathfinder.h"

#include "Path.h"

void UrogueyMovementManager::RogueyTick(uint32 TickIndex)
{
	while (MovementQueue.IsEmpty())
	{
		FMovement ProcessMovement;
		MovementQueue.Dequeue(ProcessMovement);
		if (ProcessMovement.Actor != nullptr)
		{
			FPath NewPath = UrogueyPathfinder::FindAndGeneratePath(ProcessMovement, TODO);
		}
		

	}
}

void UrogueyMovementManager::EnqueueMovement(const FMovement& Movement)
{
	MovementQueue.Enqueue(Movement);
}

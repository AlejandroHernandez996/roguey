// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyCombatManager.h"

#include "Grid/rogueyGridManager.h"
#include "Grid/Util/GridUtils.h"
#include "System/rogueyDamageCalculator.h"

void UrogueyCombatManager::RogueyTick(int32 TickIndex)
{
	while (!CombatEventQueue.IsEmpty())
	{
		FCombatEvent CombatEvent;
		CombatEventQueue.Dequeue(CombatEvent);
		
		if (CombatEvent.ToActor && CombatEvent.FromActor)
		{
			if (ActiveCombats.Contains(CombatEvent.FromActor))
			{
				ActiveCombats.Remove(CombatEvent.FromActor);
			}
			ActiveCombats.Add(CombatEvent.FromActor, CombatEvent);
		}
	}

	TSet<ArogueyPawn*> FinishedCombatActors;
	for (auto& ActiveCombat : ActiveCombats)
	{
		ArogueyPawn* FromActor = ActiveCombat.Key;
		FCombatEvent& CombatEvent = ActiveCombat.Value;

		if (!FromActor || !CombatEvent.FromActor || !GridManager->IsPawnInRange(FromActor, CombatEvent.ToActor))
		{
			FinishedCombatActors.Add(FromActor);
		}
		else
		{
			UrogueyDamageCalculator::CalculateCombat(TickIndex, CombatEvent);
			FromActor->RotateAtPawn(CombatEvent.ToActor);
		}
	}
	for (auto& FinishedActor : FinishedCombatActors)
	{
		ActiveCombats.Remove(FinishedActor);
	}
}

void UrogueyCombatManager::EnqueueCombatEvent(const FCombatEvent& CombatEvent)
{
	if (CombatEvent.FromActor && CombatEvent.ToActor)
	{
		CombatEventQueue.Enqueue(CombatEvent);
	}
}

void UrogueyCombatManager::RemoveActorFromActiveQueue(ArogueyPawn* Pawn)
{
	ActiveCombats.Remove(Pawn);
}
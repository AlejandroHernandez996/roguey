// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyCombatManager.h"

#include "DeathManager.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Grid/rogueyGridManager.h"
#include "Input/rogueyInputManager.h"
#include "System/rogueyDamageCalculator.h"

void UrogueyCombatManager::RogueyTick(int32 TickIndex)
{
	while (!CombatEventQueue.IsEmpty())
	{
		FCombatEvent CombatEvent;
		CombatEventQueue.Dequeue(CombatEvent);
		
		if (CombatEvent.ToActor && CombatEvent.FromActor)
		{
			if (CombatEvent.ToActor->PawnState == EPawnState::DEAD || CombatEvent.FromActor->PawnState == EPawnState::DEAD)
			{
				continue;
			}
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
		if (!CombatEvent.ToActor || !CombatEvent.FromActor)
		{
			continue;
		}
		if (CombatEvent.ToActor && CombatEvent.FromActor)
		{
			if (CombatEvent.ToActor->PawnState == EPawnState::DEAD || CombatEvent.FromActor->PawnState == EPawnState::DEAD)
			{
				FinishedCombatActors.Add(CombatEvent.FromActor);
				continue;
			}
		}
		bool bIsInRange = GridManager->IsPawnInRange(FromActor, CombatEvent.ToActor);
		if (!FromActor || !CombatEvent.FromActor || !bIsInRange)
		{
			if (!bIsInRange)
			{
				InputManager->EnqueueInput(FInput(TickIndex, EInputType::ATTACK,FVector::Zero(),FromActor, CombatEvent.ToActor));
			}
			FinishedCombatActors.Add(FromActor);
		}
		else if (FromActor)
		{
			UrogueyDamageCalculator::CalculateCombat(TickIndex, CombatEvent);
			FromActor->RotateAtPawn(CombatEvent.ToActor);
			if (CombatEvent.ToActor->StatPage.StatPage[ErogueyStatType::HEALTH].CurrentLevel <= 0 && !Cast<ArogueyCharacter>(CombatEvent.ToActor))
			{
				FinishedCombatActors.Add(FromActor);
				DeathManager->EnqueueDeath(CombatEvent.ToActor);
			}
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
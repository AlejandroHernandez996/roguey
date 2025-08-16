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
		
		if (CombatEvent.ToActor.Get() && CombatEvent.FromActor.Get())
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
		TWeakObjectPtr<ArogueyPawn> FromActor = ActiveCombat.Key;
		FCombatEvent& CombatEvent = ActiveCombat.Value;
		if (!CombatEvent.ToActor.Get() || !CombatEvent.FromActor.Get())
		{
			continue;
		}
		if (CombatEvent.ToActor.Get() && CombatEvent.FromActor.Get())
		{
			if (CombatEvent.ToActor->PawnState == EPawnState::DEAD || CombatEvent.FromActor->PawnState == EPawnState::DEAD)
			{
				FinishedCombatActors.Add(CombatEvent.FromActor.Get());
				continue;
			}
		}
		bool bIsInRange = GridManager->IsPawnInRange(FromActor.Get(), CombatEvent.ToActor.Get());
		if (!FromActor.Get() || !CombatEvent.FromActor.Get() || !bIsInRange)
		{
			if (!bIsInRange)
			{
				InputManager->EnqueueInput(FInput(TickIndex, EInputType::ATTACK,FVector::Zero(),FromActor.Get(), CombatEvent.ToActor.Get()));
			}
			FinishedCombatActors.Add(FromActor.Get());
		}
		else if (FromActor.Get())
		{
			UrogueyDamageCalculator::CalculateCombat(TickIndex, CombatEvent);
			FromActor->RotateAtPawn(CombatEvent.ToActor.Get());
			if (CombatEvent.ToActor->StatPage.StatPage[ErogueyStatType::HEALTH].CurrentLevel <= 0 && !Cast<ArogueyCharacter>(CombatEvent.ToActor))
			{
				FinishedCombatActors.Add(FromActor.Get());
				DeathManager->EnqueueDeath(CombatEvent.ToActor.Get());
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
	if (CombatEvent.FromActor.Get() && CombatEvent.ToActor.Get())
	{
		CombatEventQueue.Enqueue(CombatEvent);
	}
}

void UrogueyCombatManager::RemoveActorFromActiveQueue(TWeakObjectPtr<ArogueyPawn> Pawn)
{
	ActiveCombats.Remove(Pawn);
}
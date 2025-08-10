// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyCombatManager.h"

void UrogueyCombatManager::RogueyTick(uint32 TickIndex)
{
	
}

void UrogueyCombatManager::EnqueueCombatEvent(FCombatEvent CombatEvent)
{
	if (CombatEvent.FromActor && CombatEvent.ToActor)
	{
		CombatEventQueue.Enqueue(CombatEvent);
	}
}

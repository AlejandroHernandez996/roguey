// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyDamageCalculator.h"

#include "Combat/CombatEvent.h"

void UrogueyDamageCalculator::CalculateCombat(int32 TickIndex, FCombatEvent CombatEvent)
{
	ArogueyPawn* FromPawn = CombatEvent.FromActor;
	ArogueyPawn* ToPawn = CombatEvent.ToActor;

	if (TickIndex - FromPawn->LastAttackTickIndex > FromPawn->DefaultAttackCooldown)
	{
		ToPawn->UpdateCurrentStat(ErogueyStatType::HEALTH, -1);
		FromPawn->SetPawnState(EPawnState::ATTACKING, true);
		FromPawn->LastAttackTickIndex = TickIndex;
	}
}

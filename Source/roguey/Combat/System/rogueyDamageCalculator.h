// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "rogueyDamageCalculator.generated.h"

class ArogueyPawn;
struct FCombatEvent;

UCLASS()
class ROGUEY_API UrogueyDamageCalculator : public UObject
{
	GENERATED_BODY()

public:
	static void CalculateCombat(int32 TickIndex, FCombatEvent CombatEvent);

protected:
	static int32 CalculateDamage(TWeakObjectPtr<ArogueyPawn> FromPawn);
	static int32 GetDamageBonusFromStrength(int32 StrengthLevel);
	static bool CanHit(TWeakObjectPtr<ArogueyPawn> FromPawn, TWeakObjectPtr<ArogueyPawn> ToPawn);
	static int32 GetAttackRoll(TWeakObjectPtr<ArogueyPawn> FromPawn);
	static int32 GetDefenceRoll(TWeakObjectPtr<ArogueyPawn> ToPawn);
};
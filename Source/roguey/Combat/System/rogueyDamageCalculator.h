// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "rogueyDamageCalculator.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyDamageCalculator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static void CalculateCombat(int32 TickIndex, FCombatEvent CombatEvent);
};

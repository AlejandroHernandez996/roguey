// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatEvent.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "rogueyCombatManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyCombatManager : public UObject, public ITickable
{
	GENERATED_BODY()

	public:
	virtual void RogueyTick(uint32 TickIndex) override;

	TQueue<FCombatEvent> CombatEventQueue;

	UFUNCTION()
	void EnqueueCombatEvent(FCombatEvent CombatEvent);

	UPROPERTY()
	class UrogueyDamageCalculator* DamageCalculator;
	
};

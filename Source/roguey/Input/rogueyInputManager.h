// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "rogueyInputManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyInputManager : public UObject, public ITickable
{
public:
	GENERATED_BODY()
	virtual void RogueyTick(int32 TickIndex) override;
	
	TQueue<FInput> InputQueue;

	UFUNCTION()
	void EnqueueInput(const FInput& Input);

	UPROPERTY()
	class UrogueyMovementManager* MovementManager;
	UPROPERTY()
	class UrogueyCombatManager* CombatManager;
	UPROPERTY()
	class URogueyInteractManager* InteractManager;

};

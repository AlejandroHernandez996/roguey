// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "DeathManager.generated.h"

class ArogueyPawn;
/**
 * 
 */
UCLASS()
class ROGUEY_API UDeathManager : public UObject, public ITickable
{
	GENERATED_BODY()

	public:
	void RogueyTick(int32 TickIndex) override;

	TQueue<ArogueyPawn*> DeathQueue;

	UFUNCTION()
	void EnqueueDeath(ArogueyPawn* DeadPawn);

	UPROPERTY()
	class USpawnManager* SpawnManager;
	UPROPERTY()
	class UrogueyGridManager* GridManager;
};

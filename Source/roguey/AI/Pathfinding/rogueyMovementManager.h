// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPath.h"
#include "Movement.h"
#include "Path.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "rogueyMovementManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyMovementManager : public UObject, public ITickable
{
	GENERATED_BODY()
public:
	virtual void RogueyTick(int32 TickIndex) override;

	UFUNCTION()
	void EnqueueMovement(const FMovement& Movement);

	UPROPERTY()
	class UrogueyGridManager* GridManager;

	void Tick(float DeltaTime);
private:
	TQueue<FMovement> MovementQueue;

	UPROPERTY()
	TMap<AActor*, FPath> ActivePaths;

	UPROPERTY()
	TMap<AActor*, FActorPath> ActorPaths;
};

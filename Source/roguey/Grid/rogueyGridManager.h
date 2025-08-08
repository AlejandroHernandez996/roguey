// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "GridEvent.h"
#include "Chaos/PBDSuspensionConstraintData.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "rogueyGridManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyGridManager : public UObject, public ITickable
{
	GENERATED_BODY()

public:
	virtual void RogueyTick(uint32 TickIndex) override;

	UPROPERTY()
	FGrid Grid;
	TMap<ArogueyActor*, FIntVector2>* ActorMapLocation;

	UFUNCTION()
	void AddActorToGrid(ArogueyActor* Actor, FIntVector2 Location);

	UFUNCTION()
	void RemoveActorFromGrid(ArogueyActor* Actor);

	UFUNCTION()
	void MoveActor(ArogueyActor* Actor, FIntVector2 Destination);

	TQueue<FGridEvent> GridQueue;

private:
	 FIntVector2 GridSize = {64, 64};
	
};

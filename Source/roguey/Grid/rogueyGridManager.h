// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "GridEvent.h"
#include "Characters/rogueyPawn.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "Util/GridUtils.h"
#include "rogueyGridManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyGridManager : public UObject, public ITickable
{
	GENERATED_BODY()

public:
	virtual void RogueyTick(int32 TickIndex) override;
	UPROPERTY()
	FGrid Grid;

	UFUNCTION()
	void AddActorToGrid(AActor* Actor, FIntVector2 Location);

	UFUNCTION()
	void RemoveActorFromGrid(AActor* Actor);

	UFUNCTION()
	void MoveActorInGrid(AActor* Actor, FIntVector2 Destination);

	TQueue<FGridEvent> GridQueue;

	UFUNCTION()
	void EnqueueGridEvent(const FGridEvent& GridEvent);

	UFUNCTION()
	FIntVector2 GetActorTrueTile(AActor* Actor);

	UFUNCTION()
	bool GridContainsActor(AActor* Actor);

	UFUNCTION()
	bool IsPawnInRange(ArogueyPawn* From, AActor* To);
	bool IsPawnInRangeOfPoint(ArogueyPawn* FromActor, FIntVector2 FromPoint, AActor* To);

	UFUNCTION()
	void Init();

private:
	 FIntVector2 GridSize = {64, 64};
	
};

inline bool UrogueyGridManager::IsPawnInRange(ArogueyPawn* From, AActor* To)
{
	FIntVector2 FromPoint = Grid.ActorMapLocation[From];
	FIntVector2 ToPoint = Grid.ActorMapLocation[To];
	int32 CurrentRange = From->StatPage.StatPage[ErogueyStatType::ATTACK_RANGE].CurrentStat;
	int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
	if (CurrentRange == 1)
	{
		return GridUtils::IsAdjacent(FromPoint, ToPoint);
	}
	return FromPoint != ToPoint && CurrentRange >= Distance;
}

inline bool UrogueyGridManager::IsPawnInRangeOfPoint(ArogueyPawn* FromActor, FIntVector2 FromPoint, AActor* To)
{
	FIntVector2 ToPoint = Grid.ActorMapLocation[To];
	int32 CurrentRange = FromActor->StatPage.StatPage[ErogueyStatType::ATTACK_RANGE].CurrentStat;
	int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
	if (CurrentRange == 1)
	{
		return GridUtils::IsAdjacent(FromPoint, ToPoint);
	}
	return FromPoint != ToPoint && CurrentRange >= Distance;
}

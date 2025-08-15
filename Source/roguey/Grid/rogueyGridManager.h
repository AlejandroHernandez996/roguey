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
	void AddActorToGrid(ArogueyPawn* Actor, FIntVector2 Location);

	UFUNCTION()
	void RemoveActorFromGrid(ArogueyPawn* Actor);

	UFUNCTION()
	void MoveActorInGrid(ArogueyPawn* Actor, FIntVector2 Destination);

	TQueue<FGridEvent> GridQueue;

	UFUNCTION()
	void EnqueueGridEvent(const FGridEvent& GridEvent);

	UFUNCTION()
	FIntVector2 GetActorTrueTile(ArogueyPawn* Actor);

	UFUNCTION()
	bool GridContainsActor(ArogueyPawn* Actor);

	UFUNCTION()
	bool IsPawnInRange(ArogueyPawn* From, ArogueyPawn* To);
	bool IsPawnInAggroRange(ArogueyPawn* From, ArogueyPawn* To);
	bool IsPawnInRangeOfPoint(ArogueyPawn* FromActor, FIntVector2 FromPoint, ArogueyPawn* To);

	UFUNCTION()
	void Init();

	UFUNCTION()
	FIntVector2 GetPlayerTrueLocation();
	UPROPERTY()
	class ArogueyCharacter* PlayerCharacter;

private:
	 FIntVector2 GridSize = {64, 64};
	
};

inline bool UrogueyGridManager::IsPawnInRange(ArogueyPawn* From, ArogueyPawn* To)
{
	if (!Grid.ActorMapLocation.Contains(From) || !Grid.ActorMapLocation.Contains(To))
	{
		return false;
	}
	FIntVector2 FromPoint = Grid.ActorMapLocation[From];
	FIntVector2 ToPoint = Grid.ActorMapLocation[To];
	int32 CurrentRange = From->StatPage.StatPage[ErogueyStatType::ATTACK_RANGE].CurrentLevel;
	int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
	if (CurrentRange == 1)
	{
		return GridUtils::IsAdjacent(FromPoint, ToPoint);
	}
	return FromPoint != ToPoint && CurrentRange >= Distance;
}

inline bool UrogueyGridManager::IsPawnInAggroRange(ArogueyPawn* From, ArogueyPawn* To)
{
	FIntVector2 FromPoint = Grid.ActorMapLocation[From];
	FIntVector2 ToPoint = Grid.ActorMapLocation[To];
	int32 AggroRange = From->Behavior.AgressionRange;
	int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
	return FromPoint != ToPoint && AggroRange >= Distance;
}

inline bool UrogueyGridManager::IsPawnInRangeOfPoint(ArogueyPawn* FromActor, FIntVector2 FromPoint, ArogueyPawn* To)
{
	FIntVector2 ToPoint = Grid.ActorMapLocation[To];
	int32 CurrentRange = FromActor->StatPage.StatPage[ErogueyStatType::ATTACK_RANGE].CurrentLevel;
	int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
	if (CurrentRange == 1)
	{
		return GridUtils::IsAdjacent(FromPoint, ToPoint);
	}
	return FromPoint != ToPoint && CurrentRange >= Distance;
}

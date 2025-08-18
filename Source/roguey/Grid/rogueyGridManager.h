// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "GridEvent.h"
#include "rogueyGameMode.h"
#include "Characters/rogueyPawn.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "Util/GridUtils.h"
#include "rogueyGridManager.generated.h"

class ArogueyCharacter;
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
	void AddActorToGrid(TWeakObjectPtr<ArogueyPawn> Actor, FIntVector2 Location);

	UFUNCTION()
	void RemoveActorFromGrid(TWeakObjectPtr<ArogueyPawn> Actor);

	UFUNCTION()
	void MoveActorInGrid(TWeakObjectPtr<ArogueyPawn> Actor, FIntVector2 Destination);

	TQueue<FGridEvent> GridQueue;

	UFUNCTION()
	void EnqueueGridEvent(const FGridEvent& GridEvent);

	UFUNCTION()
	FIntVector2 GetActorTrueTile(TWeakObjectPtr<ArogueyPawn> Actor);

	UFUNCTION()
	bool GridContainsActor(TWeakObjectPtr<ArogueyPawn> Actor);

	UFUNCTION()
	bool IsPawnInRange(TWeakObjectPtr<ArogueyPawn> From,TWeakObjectPtr<ArogueyPawn> To);
	bool IsPawnInAggroRange(TWeakObjectPtr<ArogueyPawn> From, TWeakObjectPtr<ArogueyPawn> To);
	bool IsPawnInRangeOfPoint(TWeakObjectPtr<ArogueyPawn> FromActor, FIntVector2 FromPoint, TWeakObjectPtr<ArogueyPawn> To);

	UFUNCTION()
	void Init();

	UFUNCTION()
	FIntVector2 GetPlayerTrueLocation();
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> PlayerCharacter;

private:
	 FIntVector2 GridSize = {64, 64};
	
};

bool IsPawnInRange(TWeakObjectPtr<ArogueyPawn> From, TWeakObjectPtr<ArogueyPawn> To);

inline bool UrogueyGridManager::IsPawnInAggroRange(TWeakObjectPtr<ArogueyPawn> From, TWeakObjectPtr<ArogueyPawn> To)
{
	FIntVector2 FromPoint = Grid.ActorMapLocation[From];
	FIntVector2 ToPoint = Grid.ActorMapLocation[To];
	int32 AggroRange = From->Behavior.AgressionRange;
	int32 Distance = GridUtils::GridDistance(FromPoint, ToPoint);
	return FromPoint != ToPoint && AggroRange >= Distance;
}

inline bool UrogueyGridManager::IsPawnInRangeOfPoint(TWeakObjectPtr<ArogueyPawn> FromActor, FIntVector2 FromPoint, TWeakObjectPtr<ArogueyPawn> To)
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

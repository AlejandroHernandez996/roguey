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
	void HandleActivePaths(int32 TickIndex);
	void ProcessMovementQueue(int32 TickIndex);
	FPath GenerateNewPath(const FMovement& Movement);
	void UpdateActivePath(TWeakObjectPtr<ArogueyPawn>, const FPath& NewPath);
	void ProcessActivePaths(int32 TickIndex);
	
	UFUNCTION()
	void EnqueueMovement(const FMovement& Movement);

	UPROPERTY()
	class UrogueyGridManager* GridManager;

	UPROPERTY()
	class UrogueyCombatManager* CombatManager;

	UPROPERTY()
	class UrogueyInventoryManager* InventoryManager;

	UFUNCTION()
	void RemoveActorFromActiveQueue(TWeakObjectPtr<ArogueyPawn> Pawn);
private:
	TQueue<FMovement> MovementQueue;

	UPROPERTY()
	TMap<TWeakObjectPtr<ArogueyPawn>, FPath> ActivePaths;

	UPROPERTY()
	TMap<TWeakObjectPtr<ArogueyPawn>, FActorPath> ActorPaths;
};

#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyPawn.h"
#include "Movement.generated.h"

class ArogueyPawn;

USTRUCT(BlueprintType)
struct FMovement
{
	GENERATED_BODY()
	FMovement() {}
	
	FMovement(TWeakObjectPtr<ArogueyPawn> InActor, TWeakObjectPtr<ArogueyPawn>InTargetPawn, FIntVector2 InDestination, uint32 InTick)
		: Actor(InActor)
		, Destination(InDestination)
		, Tick(InTick)
		, TargetPawn(InTargetPawn)
	{}

	FMovement(TWeakObjectPtr<ArogueyPawn> InActor, TWeakObjectPtr<ArogueyItemActor> InTargetItem, uint32 InTick)
		: Actor(InActor)
		, Tick(InTick)
		, TargetItem(InTargetItem)
	{}
	UPROPERTY()
	FIntVector2 Destination = FIntVector2(-1, -1);
	
	UPROPERTY()
	uint32 Tick = 0;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> Actor;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> TargetPawn;

	UPROPERTY()
	TWeakObjectPtr<ArogueyItemActor> TargetItem;
};
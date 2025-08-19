#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyPawn.h"
#include "Movement.generated.h"

class ArogueyItemActor;
class AArogueyObject;
class ArogueyPawn;

USTRUCT(BlueprintType)
struct FMovement
{
	GENERATED_BODY()
	FMovement() {}
	
	FMovement(TWeakObjectPtr<ArogueyPawn> InActor, FIntVector2 InDestination, uint32 InTick)
		: Destination(InDestination)
		, Tick(InTick)
		, Actor(InActor)
	{}

	UPROPERTY()
	FIntVector2 Destination = FIntVector2(-1, -1);
	
	UPROPERTY()
	uint32 Tick = 0;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> Actor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> TargetPawn = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ArogueyItemActor> TargetItem = nullptr;
	
	UPROPERTY()
	TWeakObjectPtr<AArogueyObject> TargetObject = nullptr;
};
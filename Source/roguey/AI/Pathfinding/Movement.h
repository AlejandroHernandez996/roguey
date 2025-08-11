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
	
	FMovement(ArogueyPawn* InActor, ArogueyPawn* InTargetPawn, FIntVector2 InDestination, uint32 InTick)
		: Actor(InActor)
		, Destination(InDestination)
		, Tick(InTick)
		, TargetPawn(InTargetPawn)
	{}

	UPROPERTY()
	ArogueyPawn* Actor = nullptr;

	UPROPERTY()
	FIntVector2 Destination = FIntVector2(-1, -1);
	
	UPROPERTY()
	uint32 Tick = 0;

	UPROPERTY()
	ArogueyPawn* TargetPawn = nullptr;
};
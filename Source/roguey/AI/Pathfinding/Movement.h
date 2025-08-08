#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyActor.h"
#include "Movement.generated.h"

USTRUCT(BlueprintType)
struct FMovement
{
	GENERATED_BODY()
	FMovement(): Actor(nullptr), Destination(), Tick(0)
	{
	}

	FMovement(ArogueyActor* InActor, FIntVector2 InDestination, uint32 InTick)
		: Actor(InActor)
		, Destination(InDestination)
		, Tick(InTick)
	{}

	UPROPERTY()
	class ArogueyActor* Actor;

	UPROPERTY()
	FIntVector2 Destination;
	
	UPROPERTY()
	uint32 Tick;
};
#pragma once

#include "CoreMinimal.h"
#include "GridEventType.h"
#include "GridEvent.generated.h"

USTRUCT(BlueprintType)
struct FGridEvent
{
	GENERATED_BODY()
	UPROPERTY()
	ArogueyPawn* Actor;
	UPROPERTY()
	FIntVector2 Location;
	UPROPERTY()
	int32 Tick;
	UPROPERTY()
	EGridEventType EventType = EGridEventType::NONE;

	FGridEvent(): Actor(nullptr), Location(), Tick(0)
	{
	}

	FGridEvent(int32 InTick,const FIntVector2& InLocation, ArogueyPawn* InInputActor, EGridEventType InEventType)
		: Tick(InTick)
		, Location(InLocation)
		, Actor(InInputActor)
		, EventType(InEventType)
	{}
};
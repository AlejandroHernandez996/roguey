#pragma once

#include "CoreMinimal.h"
#include "GridEventType.h"
#include "Tile.h"
#include "GridEvent.generated.h"

USTRUCT(BlueprintType)
struct FGridEvent
{
	GENERATED_BODY()
	UPROPERTY()
	AActor* Actor;
	UPROPERTY()
	FIntVector2 Location;
	UPROPERTY()
	uint32 Tick;
	UPROPERTY()
	EGridEventType EventType = EGridEventType::NONE;

	FGridEvent(): Actor(nullptr), Location(), Tick(0)
	{
	}

	FGridEvent(uint32 InTick,const FIntVector2& InLocation, AActor* InInputActor, EGridEventType InEventType)
		: Tick(InTick)
		, Location(InLocation)
		, Actor(InInputActor)
		, EventType(InEventType)
	{}
};
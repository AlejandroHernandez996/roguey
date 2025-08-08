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
	ArogueyActor* Actor;
	UPROPERTY()
	FVector2d Location;
	UPROPERTY()
	uint32 Tick;
	UPROPERTY()
	EGridEventType EventType = EGridEventType::NONE;

	FGridEvent() {}

	FGridEvent(uint32 InTick,const FVector2d& InLocation, ArogueyActor* InInputActor, EGridEventType InEventType)
		: Tick(InTick)
		, Location(InLocation)
		, Actor(InInputActor)
		, EventType(InEventType)
	{}
};
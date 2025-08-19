#pragma once

#include "CoreMinimal.h"
#include "GridEventType.h"
#include "GridEvent.generated.h"

USTRUCT(BlueprintType)
struct FGridEvent
{
	GENERATED_BODY()
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> Actor;
	UPROPERTY()
	FIntVector2 Location;
	UPROPERTY()
	int32 Tick;
	UPROPERTY()
	EGridEventType EventType = EGridEventType::NONE;

	FGridEvent(): Actor(nullptr), Location(), Tick(0)
	{
	}

	FGridEvent(int32 InTick,const FIntVector2& InLocation, TWeakObjectPtr<ArogueyPawn> InInputActor, EGridEventType InEventType)
		: Actor(InInputActor)
		, Location(InLocation)
		, Tick(InTick)
		, EventType(InEventType)
	{}
};
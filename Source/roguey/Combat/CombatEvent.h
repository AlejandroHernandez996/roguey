#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyPawn.h"
#include "CombatEvent.generated.h"

USTRUCT(BlueprintType)
struct FCombatEvent
{
	GENERATED_BODY()
	
	int32 Tick;
	UPROPERTY()
	ArogueyPawn* FromActor;
	UPROPERTY()
	ArogueyPawn* ToActor;

	FCombatEvent(): ToActor(nullptr), FromActor(nullptr), Tick(0)
	{
	}

	FCombatEvent(ArogueyPawn* InFromActor, ArogueyPawn* InToActor, int32 InTick)
		: FromActor(InFromActor)
		, ToActor(InToActor)
		, Tick(InTick)
	{}
};
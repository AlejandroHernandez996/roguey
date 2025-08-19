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
	TWeakObjectPtr<ArogueyPawn> FromActor;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> ToActor;

	FCombatEvent(): Tick(0), FromActor(nullptr), ToActor(nullptr)
	{
	}

	FCombatEvent(TWeakObjectPtr<ArogueyPawn> InFromActor, TWeakObjectPtr<ArogueyPawn> InToActor, int32 InTick)
		: Tick(InTick)
		, FromActor(InFromActor)
		, ToActor(InToActor)
	{}
};
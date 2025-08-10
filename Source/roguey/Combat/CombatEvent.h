#pragma once

#include "CoreMinimal.h"
#include "CombatEvent.generated.h"

USTRUCT(BlueprintType)
struct FCombatEvent
{
	GENERATED_BODY()
	
	uint32 Tick;
	UPROPERTY()
	AActor* FromActor;
	UPROPERTY()
	AActor* ToActor;
};
#pragma once

#include "CoreMinimal.h"
#include "ActorPath.generated.h"

USTRUCT(BlueprintType)
struct FActorPath
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FIntVector2> MovementPath;
	UPROPERTY()
	uint32 PathIndex = 0;
};
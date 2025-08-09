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
	int32 PathIndex = 0;

	FIntVector2 GetCurrentPath()
	{
		if (PathIndex < MovementPath.Num())
		{
			return MovementPath[PathIndex];
		}
		return FIntVector2::ZeroValue;
	}
};
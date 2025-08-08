#pragma once

#include "CoreMinimal.h"
#include "Path.generated.h"

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FIntVector2> MovementPath;
	UPROPERTY()
	uint32 PathIndex = -1;

	bool IsPathComplete() const
	{
		return MovementPath.IsEmpty() || PathIndex == -1 || MovementPath.Num()-1 == PathIndex;
	}

	FIntVector2 GetMovementLocation()
	{
		if (!IsPathComplete())
		{
			return MovementPath[PathIndex];
		}
		return FIntVector2::NoneValue;
	}
};
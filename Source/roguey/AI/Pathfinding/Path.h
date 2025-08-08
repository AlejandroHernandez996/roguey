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
	int32 PathIndex = -1;

	bool IsPathComplete() const
	{
		return MovementPath.IsEmpty() || PathIndex == -1 || PathIndex == MovementPath.Num();
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
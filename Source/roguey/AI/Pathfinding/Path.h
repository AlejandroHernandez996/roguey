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
		return MovementPath.IsEmpty() || PathIndex == -1 || PathIndex >= MovementPath.Num()-1;
	}

	FIntVector2 GetAndIncrementPath(bool bIsRunning)
	{
		if (IsPathComplete())
		{
			return FIntVector2::NoneValue;
		}

		if (!bIsRunning || PathIndex == MovementPath.Num() - 2)
		{
			PathIndex++;
			return MovementPath[PathIndex];
		}

		PathIndex += 2;
		return MovementPath[PathIndex];
	}
};
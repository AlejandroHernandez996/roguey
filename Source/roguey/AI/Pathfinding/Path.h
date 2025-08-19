#pragma once

#include "CoreMinimal.h"
#include "Path.generated.h"

class ArogueyItemActor;

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FIntVector2> MovementPath;
	UPROPERTY()
	int32 PathIndex = -1;
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> TargetPawn;
	UPROPERTY()
	TWeakObjectPtr<ArogueyItemActor> TargetItem;
	UPROPERTY()
	TWeakObjectPtr<AArogueyObject> TargetObject;
	UPROPERTY()
	FIntVector2 TargetPosition = FIntVector2(-1, -1);

	bool IsPathComplete() const
	{
		return MovementPath.IsEmpty() || PathIndex == -1 || PathIndex >= MovementPath.Num()-1;
	}

	FIntVector2 GetAndIncrementPath(int32 TileSpeed)
	{
		if (IsPathComplete())
		{
			return FIntVector2::NoneValue;
		}

		int32 StepsToMove = TileSpeed;

		if (PathIndex + StepsToMove >= MovementPath.Num())
		{
			StepsToMove = 1;
		}

		PathIndex += StepsToMove;

		PathIndex = FMath::Clamp(PathIndex, 0, MovementPath.Num() - 1);

		return MovementPath[PathIndex];
	}
};
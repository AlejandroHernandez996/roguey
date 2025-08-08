#pragma once

#include "CoreMinimal.h"
#include "Grid/Tile.h"
#include "Path.generated.h"

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FTile> TilePath;
	UPROPERTY()
	uint32 PathIndex = -1;

	bool IsPathComplete() const
	{
		return TilePath.IsEmpty() || PathIndex == -1 || TilePath.Num()-1 == PathIndex;
	}
};
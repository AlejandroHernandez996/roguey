#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FGrid
{
	GENERATED_BODY()
	UPROPERTY()
	TMap<FIntVector2, FTile> GridMap;
};
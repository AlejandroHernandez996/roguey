#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyActor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()
	UPROPERTY()
	TSet<ArogueyActor*> ActorsInTile;
};
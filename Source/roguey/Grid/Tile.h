#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "TileType.h"

#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()
	UPROPERTY()
	TSet<AActor*> ActorsInTile;
	TMultiMap<int32, class ArogueyItemActor*> ItemMapInTile;
	UPROPERTY()
	TSet<class AArogueyObject*> ObjectsInTile;
	UPROPERTY()
	ETileType TileType = ETileType::FREE;

	bool IsBlocked() const
	{
		return TileType == ETileType::BLOCKED;
	}

	bool HasWall(const FIntVector2& Direction) const
	{
		switch (TileType)
		{
		case ETileType::NORTH_WALL:
			return Direction == FIntVector2(0, -1);
		case ETileType::SOUTH_WALL:
			return Direction == FIntVector2(0, 1);
		case ETileType::EAST_WALL:
			return Direction == FIntVector2(1, 0);
		case ETileType::WEST_WALL:
			return Direction == FIntVector2(-1, 0);
		default: return false;
		}
	}
};
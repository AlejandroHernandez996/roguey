#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "TileType.h"
#include "Objects/ArogueyObject.h"

#include "Tile.generated.h"

class ArogueyItemActor;

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> ActorsInTile;
	TMultiMap<int32, TWeakObjectPtr<ArogueyItemActor>> ItemMapInTile;
	UPROPERTY()
	TSet<TWeakObjectPtr<AArogueyObject>> ObjectsInTile;
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
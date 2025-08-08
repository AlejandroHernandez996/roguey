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
	UPROPERTY()
	TMap<AActor*, FIntVector2> ActorMapLocation;

	bool HasWall(const FIntVector2& TileCoordinate, const FIntVector2& Direction) const
	{
		const FTile& Tile = GridMap[TileCoordinate];
		return Tile.HasWall(Direction);
	}

	bool CanMove(const FIntVector2& From, const FIntVector2& To) const
	{
		if (!GridMap.Contains(From) || !GridMap.Contains(To))
		{
			return false;
		}
		
		const FTile& FromTile = GridMap[From];
		const FTile& ToTile = GridMap[To];

		if (ToTile.IsBlocked()) return false;

		int DX = To.X - From.X;
		int DY = To.Y - From.Y;

		DX = DX != 0 ? DX/FMath::Abs(DX) : 0;
		DY = DY != 0 ? DY/FMath::Abs(DY) : 0;

		FIntVector2 Dir(DX, DY);

		if ((FMath::Abs(DX) == 1 && DY == 0) || (FMath::Abs(DY) == 1 && DX == 0))
		{
			if (FromTile.HasWall(Dir)) return false;
			if (ToTile.HasWall(FIntVector2(-DX, -DY))) return false;
			return true;
		}

		if (FMath::Abs(DX) == 1 && FMath::Abs(DY) == 1)
		{
			if (!CanMove(From, From + FIntVector2(DX, 0))) return false;
			if (!CanMove(From, From + FIntVector2(0, DY))) return false;
			return true;
		}

		return false;
	}
};
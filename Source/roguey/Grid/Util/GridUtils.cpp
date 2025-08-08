
#include "GridUtils.h"

FIntPoint GridUtils::WorldToGrid(const FVector& WorldPosition)
{
	int32 X = FMath::FloorToInt(WorldPosition.X/TileSize);
	int32 Y = FMath::FloorToInt(WorldPosition.Y/TileSize);

	return FIntPoint(X, Y);
}

FVector GridUtils::GridToWorld(const FIntVector2& GridPosition)
{
	float X = (GridPosition.X + .5f) * TileSize;
	float Y = (GridPosition.Y + .5f) * TileSize;
	return FVector(X, Y, .0f);
}

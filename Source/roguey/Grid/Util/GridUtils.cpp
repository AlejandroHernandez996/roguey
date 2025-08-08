
#include "GridUtils.h"

FIntPoint GridUtils::WorldToGrid(const FVector2d& WorldPosition)
{
	int32 X = FMath::FloorToInt(WorldPosition.X/TileSize);
	int32 Y = FMath::FloorToInt(WorldPosition.Y/TileSize);

	return FIntPoint(X, Y);
}

FVector GridUtils::GridToWorld(const FIntPoint& GridPosition)
{
	float X = (GridPosition.X + .5f) * TileSize;
	float Y = (GridPosition.Y + .5f) * TileSize;
	return FVector(X, Y, .0f);
}

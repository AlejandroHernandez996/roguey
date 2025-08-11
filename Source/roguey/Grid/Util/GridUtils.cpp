
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

bool GridUtils::IsAdjacent(const FIntVector2 FromPoint, const FIntVector2 ToPoint)
{
	int32 DeltaX = FMath::Abs(FromPoint.X - ToPoint.X);
	int32 DeltaY = FMath::Abs(FromPoint.Y - ToPoint.Y);

	return (DeltaX == 1 && DeltaY == 0) || (DeltaX == 0 && DeltaY == 1);
}

bool GridUtils::IsOrthogonallyAdjacent(const FIntVector2& A, const FIntVector2& B)
{
	return (FMath::Abs(A.X - B.X) == 1 && A.Y == B.Y) ||
		   (FMath::Abs(A.Y - B.Y) == 1 && A.X == B.X);
}

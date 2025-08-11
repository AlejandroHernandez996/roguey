#pragma once

#include "CoreMinimal.h"

class ROGUEY_API GridUtils
{
public:
	static FIntPoint WorldToGrid(const FVector& WorldPosition);
	static FVector GridToWorld(const FIntVector2& GridPosition);
	static int32 GridDistance(const FIntVector2& A, const FIntVector2& B);
	static bool IsAdjacent(const FIntVector2 FromPoint, const FIntVector2 ToPoint);
	static bool IsOrthogonallyAdjacent(const FIntVector2& A, const FIntVector2& B);
	static constexpr uint32 TileSize = 100;
};

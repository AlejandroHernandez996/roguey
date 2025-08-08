#pragma once

#include "CoreMinimal.h"

class ROGUEY_API GridUtils
{
public:
	static FIntPoint WorldToGrid(const FVector& WorldPosition);
	static FVector GridToWorld(const FIntVector2& GridPosition);
	static constexpr uint32 TileSize = 10;
};

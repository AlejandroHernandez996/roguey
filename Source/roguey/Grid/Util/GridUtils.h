#pragma once

#include "CoreMinimal.h"

class ROGUEY_API GridUtils
{
public:
	static FIntPoint WorldToGrid(const FVector2d& WorldPosition);
	static FVector GridToWorld(const FIntPoint& GridPosition);
	static constexpr uint32 TileSize = 10;
};

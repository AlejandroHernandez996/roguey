#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	FREE = 0,
	BLOCKED = 1,
	NORTH_WALL = 2,
	SOUTH_WALL = 3,
	EAST_WALL = 4,
	WEST_WALL = 5,
};
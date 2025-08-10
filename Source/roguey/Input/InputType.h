#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	MOVE = 0,
	ATTACK = 1,
	EQUIP = 2,
	ABILITY = 3,
	EXAMINE = 4,
	PRIMARY_MOD = 5,
	SECONDARY_MOD = 6,
	NONE = 7,
};
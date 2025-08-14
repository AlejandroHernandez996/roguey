#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	MOVE = 0,
	FOLLOW = 1,
	ATTACK = 2,
	EQUIP = 3,
	ABILITY = 4,
	EXAMINE = 5,
	PRIMARY_MOD = 6,
	SECONDARY_MOD = 7,
	NONE = 8,
	PICK_UP_ITEM = 9
};
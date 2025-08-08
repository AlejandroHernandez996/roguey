#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	MOVEMENT_INPUT = 0,
	ATTACK_INPUT = 1,
	ITEM_INPUT = 2,
	ABILTY_INPUT = 3,
	PRAYER_INPUT = 4,
	NONE = 5,
};
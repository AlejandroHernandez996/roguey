#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	MELEE = 0,
	RANGE = 1,
	MAGIC = 2,
};
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGridEventType : uint8
{
	MOVE = 0,
	ADD = 1,
	REMOVE = 2,
	NONE = 3,
};
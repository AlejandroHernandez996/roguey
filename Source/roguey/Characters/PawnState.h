#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPawnState : uint8
{
	IDLE = 0,
	RUNNING = 1,
	WALKING = 2,
};
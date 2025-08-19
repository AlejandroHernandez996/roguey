#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPawnState : uint8
{
	IDLE = 0,
	MOVING = 1,
	FOLLOWING = 2,
	ATTACK_MOVING = 3,
	ATTACKING = 4,
	DEAD = 5,
	SKILLING = 6,
	SPAWNED = 7,
};
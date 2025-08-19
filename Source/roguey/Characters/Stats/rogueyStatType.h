// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ErogueyStatType : uint8
{
	FLAT_ARMOR,
	HEALTH,
	ENERGY,
	ATTACK,
	STRENGTH,
	DEFENCE,
	ATTACK_RANGE,
	ATTACK_SPEED,
	RANGING,
	MAGIC,
	WOODCUTTING,
	MINING
};
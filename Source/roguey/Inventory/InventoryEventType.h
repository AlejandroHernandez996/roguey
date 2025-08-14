// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInventoryEventType : uint8
{
	PICK_UP	= 0,
	DROP,
	EQUIP,
	USE,
	MOVE
};
// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInventoryEventType : uint8
{
	PICK_UP UMETA(DisplayName = "Pickup"),
	DROP UMETA(DisplayName = "Drop"),
	EQUIP UMETA(DisplayName = "Equip"),
	MOVE UMETA(DisplayName = "Move"),
	USE UMETA(DisplayName = "Use"),
};

FORCEINLINE FString EInventoryEventTypeToString(EInventoryEventType Event)
{
	switch (Event)
	{
		case EInventoryEventType::USE: return TEXT("Use");
		case EInventoryEventType::EQUIP: return TEXT("Equip");
		case EInventoryEventType::DROP: return TEXT("Drop");
		default: return TEXT("Unknown");
	}
}
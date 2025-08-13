#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	ATTACK UMETA(DisplayName = "Attack"),
	TAKE UMETA(DisplayName = "Take"),
	USE UMETA(DisplayName = "Use"),
	EQUIP UMETA(DisplayName = "Equip"),
	DROP UMETA(DisplayName = "Drop"),
	EAT UMETA(DisplayName = "Eat"),
	DRINK UMETA(DisplayName = "Drink")
};

FORCEINLINE FString EPawnStateToString(EInteractType State)
{
	switch (State)
	{
	case EInteractType::ATTACK: return TEXT("Attack");
	case EInteractType::TAKE: return TEXT("Take");
	case EInteractType::USE: return TEXT("Use");
	case EInteractType::EQUIP: return TEXT("Equip");
	case EInteractType::DROP: return TEXT("Drop");
	case EInteractType::EAT: return TEXT("Eat");
	case EInteractType::DRINK: return TEXT("Drink");
	default: return TEXT("Unknown");
	}
}
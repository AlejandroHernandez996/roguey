#pragma once

#include "CoreMinimal.h"
#include "rogueyEquipment.generated.h"

class UrogueyItem;
enum class EEquipmentType : uint8;

USTRUCT(BlueprintType)
struct FrogueyEquipment
{
	GENERATED_BODY()
	UPROPERTY()
	TMap<EEquipmentType, UrogueyItem*> Equipment;
};
#pragma once

#include "CoreMinimal.h"
#include "rogueyEquipment.generated.h"

class ArogueyItem;
enum class EEquipmentType : uint8;

USTRUCT(BlueprintType)
struct FrogueyEquipment
{
	GENERATED_BODY()
	UPROPERTY()
	TMap<EEquipmentType, ArogueyItem*> Equipment;
};
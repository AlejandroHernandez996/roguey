#pragma once

#include "CoreMinimal.h"
#include "Items/rogueyItem.h"
#include "rogueyEquipment.generated.h"

enum class EEquipmentType : uint8;

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyEquipment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	TMap<EEquipmentType, FrogueyItem> Equipment;
};

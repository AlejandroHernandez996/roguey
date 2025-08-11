#pragma once

#include "CoreMinimal.h"
#include "rogueyInventory.generated.h"

class ArogueyItem;

USTRUCT(BlueprintType)
struct FrogueyInventory
{
	GENERATED_BODY()
	UPROPERTY()
	TMap<int32, ArogueyItem*> Inventory;
};
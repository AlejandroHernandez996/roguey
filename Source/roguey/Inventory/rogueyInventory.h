#pragma once

#include "CoreMinimal.h"
#include "rogueyInventory.generated.h"

class UrogueyItem;

USTRUCT(BlueprintType)
struct FrogueyInventory
{
	GENERATED_BODY()
	UPROPERTY()
	TMap<int32, UrogueyItem*> Inventory;
};
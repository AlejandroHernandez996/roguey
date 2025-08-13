#pragma once

#include "CoreMinimal.h"
#include "Items/rogueyItem.h"  
#include "rogueyInventory.generated.h"

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyInventory
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TMap<int32, FrogueyItem> Inventory;
};

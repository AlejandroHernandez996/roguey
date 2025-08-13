#pragma once

#include "CoreMinimal.h"
#include "Items/rogueyItem.h"
#include "rogueyLoot.generated.h"

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyLoot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Itemid = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	FrogueyItem Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1"))
	int32 DropChance = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0"))
	int32 MinQuantity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0"))
	int32 MaxQuantity = 1;
};
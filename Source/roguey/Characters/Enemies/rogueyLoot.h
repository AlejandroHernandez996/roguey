#pragma once

#include "CoreMinimal.h"
#include "Items/rogueyItem.h"
#include "RogueyLoot.generated.h"

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyLoot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 ItemId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	FrogueyItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1"))
	int32 DropChance = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1"))
	int32 MinQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1"))
	int32 MaxQuantity = 1;

	int32 GetRandomQuantity() const
	{
		return FMath::RandRange(MinQuantity, MaxQuantity);
	}
};
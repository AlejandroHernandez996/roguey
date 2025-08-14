#pragma once

#include "CoreMinimal.h"
#include "rogueyLoot.h" 
#include "Items/rogueyItem.h" 
#include "rogueyLootTable.generated.h"

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyLootTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FrogueyLoot> ItemTable;

	FrogueyItem RollLoot() const
	{
		if (ItemTable.IsEmpty())
		{
			return FrogueyItem();
		}

		for (const FrogueyLoot& Loot : ItemTable)
		{
			if (Loot.DropChance > 0 && FMath::RandRange(1, Loot.DropChance) == 1)
			{
				FrogueyItem LootItem = Loot.Item;
				LootItem.Quantity = Loot.GetRandomQuantity();
				return LootItem; 
			}
		}

		return FrogueyItem();
	}
};

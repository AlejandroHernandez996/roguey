#pragma once

#include "CoreMinimal.h"
#include "Items/rogueyItem.h"  
#include "rogueyInventory.generated.h"

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyInventory
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Inventory_Size = 28;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FrogueyItem> Items;

	bool AddItem(FrogueyItem Item)
	{
		if (Item.bIsStackable)
		{
			for (int i = 0; i < Inventory_Size; i++)
			{
				if (Items.Contains(i) && Items[i].ItemId == Item.ItemId)
				{
					Items[i].Quantity += Item.Quantity;
					return true;
				}
			}
		}
		for (int i = 0; i < Inventory_Size; i++)
		{
			if (!Items.Contains(i))
			{
				Items.Add(i, Item);
				return true;
			}
		}
		return false;
	}
	FrogueyItem RemoveItem(int32 ItemIndex)
	{
		if (Items.Contains(ItemIndex))
		{
			return Items.FindAndRemoveChecked(ItemIndex);
		}
		return FrogueyItem();
	}
	void MoveItem(int32 FromIndex, int32 ToIndex)
	{
		if (Items.Contains(FromIndex))
		{
			FrogueyItem FromTemp = Items[FromIndex];
			if (!Items.Contains(ToIndex))
			{
				Items.Add(ToIndex, FromTemp);
				Items.Remove(FromIndex);
			}else
			{
				Items[FromIndex] = Items[ToIndex];
				Items[ToIndex] = FromTemp;
			}
		}
	}
};

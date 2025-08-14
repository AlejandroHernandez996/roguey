// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyItemCache.h"

void UrogueyItemCache::InitLootTable(ArogueyPawn* Pawn)
{
	FrogueyLootTable& LootTable = Pawn->LootTable;
	for (auto& Loot : LootTable.ItemTable)
	{
		Loot.Item = Items[Loot.ItemId];
	}
}

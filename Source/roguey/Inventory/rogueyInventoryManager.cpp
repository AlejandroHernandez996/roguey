// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyInventoryManager.h"

#include "InventoryEvent.h"
#include "Items/rogueyItemActor.h"
#include "InventoryEventType.h"
#include "Characters/SpawnManager.h"
#include "Characters/Player/Controller/rogueyPlayerController.h"
#include "Grid/rogueyGridManager.h"

void UrogueyInventoryManager::RogueyTick(int32 TickIndex)
{
	while (!InventoryEventQueue.IsEmpty())
	{
		FInventoryEvent InventoryEvent;
		InventoryEventQueue.Dequeue(InventoryEvent);

		if (InventoryEvent.EventType == EInventoryEventType::PICK_UP)
		{
			PickUpItem(InventoryEvent.ItemActor);
		}
		if (InventoryEvent.EventType == EInventoryEventType::DROP)
		{
			DropItem(InventoryEvent.FromIndex);
		}
	}
}

void UrogueyInventoryManager::EnqueueIventoryEvent(FInventoryEvent InventoryEvent)
{
	InventoryEventQueue.Enqueue(InventoryEvent);
}

void UrogueyInventoryManager::PickUpItem(TWeakObjectPtr<ArogueyItemActor> ItemToPickup)
{
	if ( ItemToPickup.IsValid() && !ItemToPickup->bPickedUp)
	{
		if (Inventory.AddItem(ItemToPickup->Item))
		{
			ItemToPickup->bPickedUp = true;
			RogueyPlayerController->OnInventoryUpdate.Broadcast(Inventory);
			TArray<TWeakObjectPtr<ArogueyItemActor>> ItemToPickupActors;
			GridManager->Grid.GridMap[ItemToPickup->Item.SpawnGridPosition].ItemMapInTile.MultiFind(ItemToPickup->Item.ItemId, ItemToPickupActors);
			GridManager->Grid.GridMap[ItemToPickup->Item.SpawnGridPosition].ItemMapInTile.Remove(ItemToPickup->Item.ItemId);
			for (auto& ItemToPickupActor : ItemToPickupActors)
			{
				if (ItemToPickupActor == ItemToPickup) continue;
				GridManager->Grid.GridMap[ItemToPickup->Item.SpawnGridPosition].ItemMapInTile.Add(ItemToPickup->Item.ItemId, ItemToPickupActor);
			}
			ItemToPickup->Destroy();
		}
	}
}

void UrogueyInventoryManager::DropItem(int32 InventoryIndex)
{
	FrogueyItem DroppedItem = Inventory.RemoveItem(InventoryIndex);
	if (DroppedItem.ItemId == -1) return;
	DroppedItem.SpawnGridPosition = GridManager->GetPlayerTrueLocation();
	SpawnManager->EnqueueItem(DroppedItem);
	RogueyPlayerController->OnInventoryUpdate.Broadcast(Inventory);
}

void UrogueyInventoryManager::EquipItem(int32 InventoryIndex)
{
}

void UrogueyInventoryManager::UnenquipItem(EEquipmentType EquipmentType)
{
}

FrogueyItem UrogueyInventoryManager::GetItemAt(int32 Index)
{
	return Inventory.Items.Contains(Index) ? Inventory.Items[Index] : FrogueyItem();
}

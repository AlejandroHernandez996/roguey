// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyInventoryManager.h"

#include "InventoryEvent.h"
#include "Items/rogueyItemActor.h"
#include "InventoryEventType.h"
#include "Characters/Player/Controller/rogueyPlayerController.h"

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
	}
}

void UrogueyInventoryManager::EnqueueIventoryEvent(FInventoryEvent InventoryEvent)
{
	InventoryEventQueue.Enqueue(InventoryEvent);
}

void UrogueyInventoryManager::PickUpItem(ArogueyItemActor* ItemToPickup)
{
	if (Inventory.AddItem(ItemToPickup->Item))
	{
		ItemToPickup->Destroy();
		RogueyPlayerController->OnInventoryUpdate.Broadcast(Inventory);
	}
}

void UrogueyInventoryManager::DropItem(int32 InventoryIndex)
{
}

void UrogueyInventoryManager::EquipItem(int32 InventoryIndex)
{
}

void UrogueyInventoryManager::UnenquipItem(EEquipmentType EquipmentType)
{
}

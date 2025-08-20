// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyInventoryManager.h"

#include "InventoryEvent.h"
#include "Items/rogueyItemActor.h"
#include "InventoryEventType.h"
#include "Characters/SpawnManager.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Characters/Player/Controller/rogueyPlayerController.h"
#include "Grid/rogueyGridManager.h"

void UrogueyInventoryManager::RogueyTick(int32 TickIndex)
{
	while (!InventoryEventQueue.IsEmpty())
	{
		FInventoryEvent InventoryEvent;
		InventoryEventQueue.Dequeue(InventoryEvent);

		if (InventoryEvent.EventType == EInventoryEventType::EQUIP)
		{
			EquipItem(InventoryEvent.FromIndex);
		}
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
	if (!Inventory.Items.Contains(InventoryIndex))
	{
		return;
	}
	
	FrogueyItem ItemToEquip = Inventory.Items[InventoryIndex];
	
	if (ItemToEquip.EquipmentSlot == EEquipmentType::NONE)
	{
		return;
	}
	FrogueyItem PreviousEquippedItem = FrogueyItem();
	if (Equipment.Equipment.Contains(ItemToEquip.EquipmentSlot))
	{
		PreviousEquippedItem = Equipment.Equipment[ItemToEquip.EquipmentSlot];
	}
	Equipment.Equipment.Add(ItemToEquip.EquipmentSlot, ItemToEquip);
	if (ItemToEquip.WeaponAttackAnimation)
	{
		RogueyPlayerController->GetCharacter()->AttackMontage = ItemToEquip.WeaponAttackAnimation;
	}

	Inventory.RemoveItem(InventoryIndex);
	if (PreviousEquippedItem.ItemId != -1)
	{
		Inventory.Items.Add(InventoryIndex, PreviousEquippedItem);
	}
	
	RogueyPlayerController->OnInventoryUpdate.Broadcast(Inventory);
	Cast<ArogueyCharacter>(RogueyPlayerController->GetPawn())->EquipItemMesh(ItemToEquip);
}

void UrogueyInventoryManager::UnenquipItem(EEquipmentType EquipmentType)
{
}

FrogueyItem UrogueyInventoryManager::GetItemAt(int32 Index)
{
	return Inventory.Items.Contains(Index) ? Inventory.Items[Index] : FrogueyItem();
}

int32 UrogueyInventoryManager::GetTotalBonusByStat(EItemStatType ItemStatType)
{
	int32 TotalBonus = 0;
	for (auto& EquippedItem : Equipment.Equipment)
	{
		if (EquippedItem.Value.ItemStatPage.StatPage.Contains(ItemStatType))
		{
			TotalBonus += EquippedItem.Value.ItemStatPage.StatPage[ItemStatType].BaseStat;
		}
	}

	return TotalBonus;
}

TSubclassOf<ArogueyProjectile> UrogueyInventoryManager::GetProjectileClass()
{
	if (Equipment.Equipment.Contains(EEquipmentType::WEAPON))
	{
		return Equipment.Equipment[EEquipmentType::WEAPON].ItemProjectileClass;
	}
	return nullptr;
}

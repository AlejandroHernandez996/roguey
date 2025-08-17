// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"

#include "Grid/rogueyGridManager.h"
#include "Grid/Util/GridUtils.h"
#include "Items/rogueyItemActor.h"
#include "Items/rogueyItemCache.h"
#include "Kismet/GameplayStatics.h"

void USpawnManager::RogueyTick(int32 TickIndex)
{
	while (!ItemSpawnQueue.IsEmpty())
	{
		FrogueyItem ItemToSpawn;
		ItemSpawnQueue.Dequeue(ItemToSpawn);

		if (TSubclassOf<ArogueyItemActor> ItemActor = ItemToSpawn.ItemActor)
		{
			bool bIsValidSpawnTile = GridManager->Grid.GridMap.Contains(ItemToSpawn.SpawnGridPosition);
			bool bTileContainsItem = GridManager->Grid.GridMap[ItemToSpawn.SpawnGridPosition].ItemMapInTile.Contains(ItemToSpawn.ItemId);
			bool bItemIsStackable = ItemToSpawn.bIsStackable;
			if ( bIsValidSpawnTile && bTileContainsItem && bItemIsStackable)
			{
				TWeakObjectPtr<ArogueyItemActor> GroundItem = GridManager->Grid.GridMap[ItemToSpawn.SpawnGridPosition].ItemMapInTile.FindArbitraryElement()->Value;
				GroundItem->Item.Quantity += ItemToSpawn.Quantity;
			} 
			else if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 

				FVector SpawnLocation = GridUtils::GridToWorld(ItemToSpawn.SpawnGridPosition);
				FRotator SpawnRotation = FRotator::ZeroRotator;

				TWeakObjectPtr<ArogueyItemActor> SpawnedActor = World->SpawnActorDeferred<ArogueyItemActor>(ItemActor.Get(), FTransform(SpawnRotation, SpawnLocation), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				if (SpawnedActor.Get() && ItemToSpawn.ItemMesh)
				{
					SpawnedActor->MeshComponent->SetStaticMesh(ItemToSpawn.ItemMesh);
					SpawnedActor->MeshComponent->SetRelativeScale3D(ItemToSpawn.MeshRelativeScale);
					SpawnedActor->MeshComponent->SetRelativeLocation(ItemToSpawn.MeshRelativeLocation);
					SpawnedActor->MeshComponent->SetRelativeRotation(ItemToSpawn.MeshRelativeRotation);
					SpawnedActor->Item = ItemToSpawn;
					UGameplayStatics::FinishSpawningActor(SpawnedActor.Get(), FTransform(SpawnRotation, SpawnLocation));
					GridManager->Grid.GridMap[ItemToSpawn.SpawnGridPosition].ItemMapInTile.Add({ItemToSpawn.ItemId, SpawnedActor});
				}
			}
		}
	}
}

void USpawnManager::EnqueueItem(FrogueyItem Item)
{
	ItemSpawnQueue.Enqueue(Item);
}

void USpawnManager::EnqueueLootItem(FrogueyLoot Loot)
{
	FrogueyItem Item = ItemCache->Items[Loot.ItemId];
	Item.Quantity = Loot.Item.Quantity;
	EnqueueItem(Item);
}

void USpawnManager::EnqueuePawn(TSubclassOf<ArogueyPawn> Pawn)
{
}

void USpawnManager::InitLootTable(ArogueyPawn* Pawn)
{
}

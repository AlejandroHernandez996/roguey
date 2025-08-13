// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"

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
			if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 

				FVector SpawnLocation = GridUtils::GridToWorld(ItemToSpawn.SpawnGridPosition);
				FRotator SpawnRotation = FRotator::ZeroRotator;

				ArogueyItemActor* SpawnedActor = World->SpawnActorDeferred<ArogueyItemActor>(ItemActor.Get(), FTransform(SpawnRotation, SpawnLocation), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				if (SpawnedActor && ItemToSpawn.ItemMesh)
				{
					SpawnedActor->MeshComponent->SetStaticMesh(ItemToSpawn.ItemMesh);
					SpawnedActor->Item = ItemToSpawn;
					UGameplayStatics::FinishSpawningActor(SpawnedActor, FTransform(SpawnRotation, SpawnLocation));
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
	FrogueyItem Item = ItemCache->Items[Loot.Itemid];
	Item.Quantity = Loot.Item.Quantity;
	EnqueueItem(Item);
}

void USpawnManager::EnqueuePawn(TSubclassOf<ArogueyPawn> Pawn)
{
}

void USpawnManager::InitLootTable(ArogueyPawn* Pawn)
{
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "rogueyEquipment.h"
#include "rogueyInventory.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "rogueyInventoryManager.generated.h"

class ArogueyPlayerController;
struct FInventoryEvent;
class ArogueyCharacter;
/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyInventoryManager : public UObject, public ITickable
{
	GENERATED_BODY()
public:
	void RogueyTick(int32 TickIndex) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FrogueyInventory Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FrogueyEquipment Equipment;
	UPROPERTY()
	ArogueyPlayerController* RogueyPlayerController;
	TQueue<FInventoryEvent> InventoryEventQueue;
	UFUNCTION()
	void EnqueueIventoryEvent(FInventoryEvent InventoryEvent);
	UFUNCTION()
	void PickUpItem(ArogueyItemActor* PickupItem);
	UFUNCTION()
	void DropItem(int32 InventoryIndex);
	UFUNCTION()
	void EquipItem(int32 InventoryIndex);
	UFUNCTION()
	void UnenquipItem(EEquipmentType EquipmentType);
	
};

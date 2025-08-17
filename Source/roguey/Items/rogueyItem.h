#pragma once

#include "CoreMinimal.h"
#include "ItemStatPage.h"
#include "Combat/Projectile/rogueyProjectile.h"
#include "Inventory/EquipmentType.h"
#include "Inventory/InventoryEventType.h"
#include "rogueyItem.generated.h"

enum class EInventoryEventType : uint8;
enum class EInteractType : uint8;

USTRUCT(BlueprintType)
struct ROGUEY_API FrogueyItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStatPage ItemStatPage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ArogueyProjectile> ItemProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ItemLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ItemRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ArogueyItemActor> ItemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector2 SpawnGridPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshRelativeScale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshRelativeLocation = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MeshRelativeRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EquipMeshRelativeScale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EquipMeshRelativeLocation = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator EquipMeshRelativeRotation = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentType EquipmentSlot = EEquipmentType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EInventoryEventType> Interacts  = {EInventoryEventType::DROP};
};
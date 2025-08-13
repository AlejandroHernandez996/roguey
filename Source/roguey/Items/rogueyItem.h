#pragma once

#include "CoreMinimal.h"
#include "ItemStatPage.h"
#include "Combat/Projectile/rogueyProjectile.h"
#include "rogueyItem.generated.h"

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
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh;

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
};
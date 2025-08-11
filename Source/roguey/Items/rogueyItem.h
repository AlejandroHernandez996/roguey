#pragma once

#include "CoreMinimal.h"
#include "ItemStatPage.h"
#include "Combat/Projectile/rogueyProjectile.h"
#include "rogueyItem.generated.h"

struct UItemStatPage;

UCLASS(Blueprintable)
class UrogueyItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStatPage ItemStatPage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UrogueyProjectile* ItemProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ItemLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ItemRotation;
};

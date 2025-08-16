#pragma once

#include "CoreMinimal.h"
#include "InteractTypeArray.generated.h"

enum class EInteractType : uint8;

USTRUCT(BlueprintType)
struct FInteractTypeArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	FrogueyItem InventoryItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> InteractableActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EInteractType> Array;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
};
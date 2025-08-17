#pragma once

#include "CoreMinimal.h"
#include "InteractTypeArray.generated.h"

enum class EInteractType : uint8;

USTRUCT(BlueprintType)
struct FInteractTypeArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EInventoryEventType> InventoryEvents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventoryIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> InteractableActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EInteractType> Array;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
};
#pragma once

#include "CoreMinimal.h"
#include "InventoryEvent.generated.h"

enum class EInventoryEventType : uint8;
class ArogueyItemActor;

USTRUCT(BlueprintType)
struct ROGUEY_API FInventoryEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ArogueyItemActor* ItemActor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FrogueyItem InventoryItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInventoryEventType EventType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 FromIndex = -1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ToIndex = -1;
};

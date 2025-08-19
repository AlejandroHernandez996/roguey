#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "rogueyItem.h"
#include "Characters/rogueyPawn.h"
#include "rogueyItemCache.generated.h"

UCLASS(BlueprintType)
class ROGUEY_API UrogueyItemCache : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Items")
	TMap<int32, FrogueyItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ArogueyItemActor> ItemActor;
	
	UFUNCTION()
	void InitLootTable(ArogueyPawn* Pawn);
};

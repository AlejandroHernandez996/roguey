#pragma once

#include "CoreMinimal.h"
#include "ItemStat.generated.h"

USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 BaseStat = 0;
};
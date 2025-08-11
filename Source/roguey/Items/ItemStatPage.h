#pragma once

#include "CoreMinimal.h"
#include "ItemStat.h"
#include "ItemStatType.h"
#include "ItemStatPage.generated.h"

USTRUCT(BlueprintType)
struct FItemStatPage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<EItemStatType, FItemStat> StatPage;
};
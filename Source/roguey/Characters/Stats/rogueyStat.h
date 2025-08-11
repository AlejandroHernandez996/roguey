#pragma once

#include "CoreMinimal.h"
#include "rogueyStat.generated.h"

USTRUCT(BlueprintType)
struct FrogueyStat
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CurrentStat = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 BaseStat = 0;
};
#pragma once

#include "CoreMinimal.h"
#include "rogueyStat.generated.h"

USTRUCT(BlueprintType)
struct FrogueyStat
{
	GENERATED_BODY()
private:
	UPROPERTY()
	int32 CurrentStat = 0;
	UPROPERTY()
	int32 BaseStat = 0;
};
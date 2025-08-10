#pragma once

#include "CoreMinimal.h"
#include "rogueyStat.h"
#include "rogueyStatType.h"
#include "rogueyStatPage.generated.h"

USTRUCT(BlueprintType)
struct FrogueyStatPage
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ErogueyStatType, FrogueyStat> StatPage;
};
#pragma once

#include "CoreMinimal.h"
#include "BehaviorType.h"
#include "Behavior.generated.h"

enum class EBehaviorType : uint8;

USTRUCT(BlueprintType)
struct FBehavior
{
	GENERATED_BODY()
	FBehavior(): BehaviorType()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBehaviorType BehaviorType = EBehaviorType::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AgressionRange = 5;
};
#pragma once

#include "CoreMinimal.h"
#include "InteractEvent.generated.h"

class AArogueyObject;
class ArogueyPawn;
enum class EInteractType : uint8;
enum class EInteractEvent : uint8;

USTRUCT(BlueprintType)
struct FInteractEvent
{
	GENERATED_BODY()

	UPROPERTY()
	EInteractType InteractType;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> InputActor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AArogueyObject> TargetObject = nullptr;
};
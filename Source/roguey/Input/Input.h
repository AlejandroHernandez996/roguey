#pragma once

#include "CoreMinimal.h"
#include "InputType.h"
#include "Input.generated.h"

USTRUCT(BlueprintType)
struct FInput
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 InputTick = 0;

	UPROPERTY()
	EInputType InputType = EInputType::NONE;

	FVector InputWorldLocation = FVector::ZeroVector;

	UPROPERTY()
	APawn* InputActor = nullptr;

	FInput() {}

	FInput(uint32 InInputTick, EInputType InInputType, const FVector& InInputWorldLocation, APawn* InInputActor)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputWorldLocation(InInputWorldLocation)
		, InputActor(InInputActor)
	{}
};
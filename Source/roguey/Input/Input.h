#pragma once

#include "CoreMinimal.h"
#include "InputType.h"
#include "Input.generated.h"

class ArogueyPawn;

USTRUCT(BlueprintType)
struct FInput
{
	GENERATED_BODY()

	UPROPERTY()
	int32 InputTick = 0;

	UPROPERTY()
	EInputType InputType = EInputType::NONE;

	FVector InputWorldLocation = FVector::ZeroVector;

	UPROPERTY()
	ArogueyPawn* InputActor = nullptr;

	UPROPERTY()
	ArogueyPawn* TargetPawn = nullptr;

	FInput() {}

	FInput(int32 InInputTick, EInputType InInputType, const FVector& InInputWorldLocation, ArogueyPawn* InInputActor, ArogueyPawn* InTargetPawn)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputWorldLocation(InInputWorldLocation)
		, InputActor(InInputActor)
		, TargetPawn(InTargetPawn)
	{}
};
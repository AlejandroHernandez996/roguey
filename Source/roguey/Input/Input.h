#pragma once

#include "CoreMinimal.h"
#include "InputType.h"
#include "Characters/rogueyActor.h"
#include "Characters/Player/rogueyCharacter.h"
#include "Input.generated.h"

USTRUCT(BlueprintType)
struct FInput
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 InputTick = 0;

	UPROPERTY()
	EInputType InputType = EInputType::NONE;

	FVector2d InputWorldLocation = FVector2d::ZeroVector;

	UPROPERTY()
	ArogueyActor* InputActor = nullptr;

	FInput() {}

	FInput(uint32 InInputTick, EInputType InInputType, const FVector2d& InInputWorldLocation, ArogueyActor* InInputActor)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputWorldLocation(InInputWorldLocation)
		, InputActor(InInputActor)
	{}
};
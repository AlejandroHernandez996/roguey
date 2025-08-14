#pragma once

#include "CoreMinimal.h"
#include "InputType.h"
#include "Input.generated.h"

class ArogueyPawn;
class ArogueyItemActor;

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

	UPROPERTY()
	ArogueyItemActor* TargetItem = nullptr;

	FInput() {}

	FInput(int32 InInputTick, EInputType InInputType, const FVector& InInputWorldLocation, ArogueyPawn* InInputActor, ArogueyPawn* InTargetPawn)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputWorldLocation(InInputWorldLocation)
		, InputActor(InInputActor)
		, TargetPawn(InTargetPawn)
	{}
	
	FInput(int32 InInputTick, EInputType InInputType, ArogueyPawn* InInputActor, ArogueyItemActor* InTargetItem)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputActor(InInputActor)
		, TargetItem(InTargetItem)
	{}
};
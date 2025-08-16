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
	TWeakObjectPtr<ArogueyPawn> InputActor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> TargetPawn = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ArogueyItemActor> TargetItem = nullptr;

	FInput() {}

	FInput(int32 InInputTick, EInputType InInputType, const FVector& InInputWorldLocation, TWeakObjectPtr<ArogueyPawn> InInputActor, TWeakObjectPtr<ArogueyPawn> InTargetPawn)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputWorldLocation(InInputWorldLocation)
		, InputActor(InInputActor)
		, TargetPawn(InTargetPawn)
	{}
	
	FInput(int32 InInputTick, EInputType InInputType, TWeakObjectPtr<ArogueyPawn> InInputActor, TWeakObjectPtr<ArogueyItemActor> InTargetItem)
		: InputTick(InInputTick)
		, InputType(InInputType)
		, InputActor(InInputActor)
		, TargetItem(InTargetItem)
	{}
};
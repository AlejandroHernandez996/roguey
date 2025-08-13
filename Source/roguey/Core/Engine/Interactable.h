#pragma once

#include "Interactable.generated.h"

enum class EInteractType : uint8;

UINTERFACE(BlueprintType, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class ROGUEY_API IInteractable
{
	GENERATED_BODY()
public:
	virtual const TArray<EInteractType>& GetInteractList() const = 0;
	virtual const FString GetRogueyName() const = 0;
	virtual const FString GetExamineText() const = 0;
};

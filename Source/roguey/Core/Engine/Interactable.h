#pragma once

#include "Interactable.generated.h"

enum class EInteractType : uint8;

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class ROGUEY_API IInteractable
{
	GENERATED_BODY()
public:
	virtual const TArray<EInteractType>& GetInteractList() const = 0;
};

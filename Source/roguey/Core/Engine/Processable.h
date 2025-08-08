#pragma once

#include "Processable.generated.h"

UINTERFACE()
class UProcessable : public UInterface
{
	GENERATED_BODY()
};

class ROGUEY_API IProcessable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Process(float DeltaTime) = 0;
};

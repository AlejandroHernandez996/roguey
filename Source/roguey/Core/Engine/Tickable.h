#pragma once

#include "Tickable.generated.h"

UINTERFACE()
class UTickable : public UInterface
{
	GENERATED_BODY()
};

class ROGUEY_API ITickable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void RogueyTick(int32 TickIndex) = 0;
};

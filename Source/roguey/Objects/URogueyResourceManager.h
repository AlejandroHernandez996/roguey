// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "URogueyResourceManager.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UURogueyResourceManager : public UObject, public ITickable
{
	GENERATED_BODY()
public:

	virtual void RogueyTick(int32 TickIndex) override;

	UPROPERTY()
	class UrogueyGridManager* GridManager;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Movement.h"
#include "UObject/Object.h"
#include "Path.h"
#include "Grid/Grid.h"
#include "rogueyPathfinder.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyPathfinder : public UObject
{
	GENERATED_BODY()
	public:
	static FPath FindAndGeneratePath(FMovement Movement, FGrid Grid);
	static FPath FindAndGeneratePathToPawn(FMovement Movement, FGrid Grid);
};

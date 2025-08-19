// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NpcCache.generated.h"

class ArogueyPawn;
/**
 * 
 */
UCLASS()
class ROGUEY_API UNpcCache : public UPrimaryDataAsset
{
	GENERATED_BODY()

	TMap<int32, TSubclassOf<ArogueyPawn>> NpcMap;
	
};

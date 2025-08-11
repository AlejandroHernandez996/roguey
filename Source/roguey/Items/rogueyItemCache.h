// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "rogueyItemCache.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API UrogueyItemCache : public UObject
{
	GENERATED_BODY()
	public:
	void LoadItems();
};

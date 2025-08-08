// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "rogueyActor.generated.h"

UCLASS()
class ROGUEY_API ArogueyActor : public AActor
{
	GENERATED_BODY()

public:
	ArogueyActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

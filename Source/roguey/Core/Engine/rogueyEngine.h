// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Processable.h"
#include "Tickable.h"
#include "Grid/rogueyGridManager.h"
#include "UObject/Object.h"
#include "rogueyEngine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickCount, const int32&, TickerCounter);

UCLASS()
class ROGUEY_API UrogueyEngine : public UObject, public IProcessable, public ITickable
{
	GENERATED_BODY()

	public:
	virtual void Process(float DeltaTime) override;
	virtual void RogueyTick(int32 TickIndex) override;

	UFUNCTION()
	void Init(const TArray<TScriptInterface<ITickable>>& ManagerArray)
	{
		this->Managers = ManagerArray;
	}

	UFUNCTION()
	uint32 GetCurrentTick() const
	{
		return GameTickCounter;
	}
	private:
	const float BASE_TICK_INTERVAL = .6f;
	float TickAccumulator = 0;
	int32 GameTickCounter = 0;

	UPROPERTY()
	TArray<TScriptInterface<ITickable>> Managers;
	UPROPERTY()
	UrogueyGridManager* GridManager;

	UPROPERTY(BlueprintAssignable, Category = "Tick")
	FTickCount OnTickCount;

};

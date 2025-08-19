// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "rogueyGameMode.h"
#include "Core/Engine/Tickable.h"
#include "Items/rogueyItemCache.h"
#include "InteractEvent.h"
#include "UObject/Object.h"
#include "RogueyInteractManager.generated.h"

enum class EInteractType : uint8;
struct FInteractEvent;
class ISkillProcessor;
class ArogueyPawn;

UCLASS()
class ROGUEY_API URogueyInteractManager : public UObject, public ITickable
{
	GENERATED_BODY()

	public:
	
	virtual void RogueyTick(int32 TickIndex) override;
	void Init(class UrogueyItemCache* ItemCache);
	void RemoveActorFromActive(TWeakObjectPtr<ArogueyPawn> Actor);

	UPROPERTY()
	TMap<EInteractType, TScriptInterface<ISkillProcessor>> SkillProcessors;
	
	TQueue<FInteractEvent> InteractQueue;

	UPROPERTY()
	TMap<TWeakObjectPtr<ArogueyPawn>, FInteractEvent> ActiveInteracts;
	void EnqueueInteractEvent(FInteractEvent InteractEvent);
};

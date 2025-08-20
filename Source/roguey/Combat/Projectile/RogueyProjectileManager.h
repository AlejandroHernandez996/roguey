// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Engine/Tickable.h"
#include "UObject/Object.h"
#include "RogueyProjectileManager.generated.h"

class ArogueyProjectile;
struct FRogueyProjectileEvent;
/**
 * 
 */
UCLASS()
class ROGUEY_API URogueyProjectileManager : public UObject, public ITickable
{
	GENERATED_BODY()

public:
	virtual void RogueyTick(int32 TickIndex) override;
	
	TQueue<FRogueyProjectileEvent> ProjectileQueue;
	UPROPERTY()
	TSet<TWeakObjectPtr<ArogueyProjectile>> ActiveProjectiles;

	UPROPERTY()
	class UrogueyGridManager* GridManager;

	UFUNCTION()
	void ProcessActiveProjectiles(int32 TickIndex);
	UFUNCTION()
	ArogueyProjectile* CreateProjectile(FRogueyProjectileEvent Event);
	UFUNCTION()
	void EnqueueProjectileEvent(FRogueyProjectileEvent Event);
};
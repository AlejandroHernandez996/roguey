// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "rogueyProjectile.generated.h"

class ArogueyPawn;
/**
 * 
 */
UCLASS()
class ROGUEY_API ArogueyProjectile : public AActor
{
	GENERATED_BODY()

public:
	ArogueyProjectile();
	
	virtual void Tick(float DeltaTime) override;
	void InitializeMovement();
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> OwningPawn;
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> TargetPawn;
	UPROPERTY()
	int32 ProjectileDamage = 0;
	UPROPERTY()
	int32 SpawnTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TravelTimeInTicks = 1;
	UPROPERTY()
	int32 TileDistance;
	float ProjectileSpeed;

	UFUNCTION()
	void MoveToTarget(float DeltaTime);
	UFUNCTION()
	void FaceTarget();
	UFUNCTION()
	void ProcessHit();
};
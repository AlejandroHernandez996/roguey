// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "rogueyPawn.h"
#include "GameFramework/Actor.h"
#include "rogueyNpcSpawner.generated.h"

class UrogueyGridManager;
UCLASS()
class ROGUEY_API ArogueyNpcSpawner : public AActor
{
	GENERATED_BODY()

public:
	ArogueyNpcSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<ArogueyPawn> CurrentSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ArogueyPawn> SpawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RespawnTimeInTicks = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimeOfDeathTick = 0;

	UFUNCTION()
	bool ShouldSpawn(int32 TickIndex)
	{
		if (CurrentSpawn.IsValid() || TimeOfDeathTick == -1)
		{
			return false;
		}
		const int32 ElapsedTicks = TickIndex - TimeOfDeathTick;
		return ElapsedTicks >= RespawnTimeInTicks;
	}
	
	UFUNCTION()
	void SetTimeOfDeath(int32 TickIndex)
	{
		TimeOfDeathTick = TickIndex;
		CurrentSpawn.Reset();
	}
	
	UFUNCTION()
	void SpawnNpc(UrogueyGridManager* GridManager);
};

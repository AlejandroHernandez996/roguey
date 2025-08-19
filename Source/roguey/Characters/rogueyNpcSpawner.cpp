// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyNpcSpawner.h"
#include "Grid/rogueyGridManager.h"
#include "Grid/Util/GridUtils.h"

ArogueyNpcSpawner::ArogueyNpcSpawner()
{
}

void ArogueyNpcSpawner::SpawnNpc(UrogueyGridManager* GridManager)
{
	if (SpawnClass)
	{
		FIntVector2 SpawnGridLocation = GridUtils::WorldToGrid(GetActorLocation());
		bool bIsValidSpawnTile = GridManager->Grid.GridMap.Contains(SpawnGridLocation);
		if (!bIsValidSpawnTile)
		{
			return;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 

		FVector SpawnLocation = GridUtils::GridToWorld(SpawnGridLocation);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (ArogueyPawn* SpawnedActor = GetWorld()->SpawnActor<ArogueyPawn>(SpawnClass.Get(), FTransform(SpawnRotation, SpawnLocation)))
		{
			GridManager->AddActorToGrid(SpawnedActor, SpawnGridLocation);
			CurrentSpawn = SpawnedActor;
			TimeOfDeathTick = -1;
			SpawnedActor->OnPawnDeath.AddDynamic(this, &ArogueyNpcSpawner::SetTimeOfDeath);
		}
	}
}


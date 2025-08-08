// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyActor.h"

#include "rogueyGameMode.h"
#include "AI/Pathfinding/rogueyMovementManager.h"
#include "Grid/Util/GridUtils.h"


ArogueyActor::ArogueyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ArogueyActor::BeginPlay()
{
	Super::BeginPlay();
	Cast<ArogueyGameMode>(GetWorld()->GetAuthGameMode())->MovementManager->GridManager->AddActorToGrid(this, GridUtils::WorldToGrid(GetTransform().GetLocation()));
}

void ArogueyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


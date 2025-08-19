// Fill out your copyright notice in the Description page of Project Settings.


#include "URogueyResourceManager.h"

#include "Grid/rogueyGridManager.h"

void UURogueyResourceManager::RogueyTick(int32 TickIndex)
{
	TSet<TWeakObjectPtr<AArogueyObject>> Objects;
	GridManager->Grid.ObjectMapLocation.GetKeys(Objects);

	for (auto& Object : Objects)
	{
		if (!Object.IsValid())
		{
			continue;
		}
		if (Object->bIsDepleted && TickIndex >= Object->DepletedOnTick+Object->RespawnTicks)
		{
			Object->RespawnResource();
		}
	}

}

// Fill out your copyright notice in the Description page of Project Settings.

#include "rogueyEngine.h"

void UrogueyEngine::Process(float DeltaTime)
{
	TickAccumulator += DeltaTime;
	if (TickAccumulator >= BASE_TICK_INTERVAL)
	{
		TickAccumulator = 0.0f;
		GameTickCounter += 1;
		OnTickCount.Broadcast(GameTickCounter);
		RogueyTick(GameTickCounter);
	}
}

void UrogueyEngine::RogueyTick(int32 TickIndex)
{
	for (const TScriptInterface Manager : Managers)
	{
		Manager->RogueyTick(GameTickCounter);
	}
}
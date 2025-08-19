// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueyInteractManager.h"

#include "InteractEvent.h"
#include "Core/Engine/InteractType.h"
#include "Skills/MiningSkillProcesor.h"
#include "Skills/WoodcuttingSkillProcessor.h"

void URogueyInteractManager::Init(UrogueyItemCache* ItemCache)
{
	UMiningSkillProcesor* MiningSkillProcessor = NewObject<UMiningSkillProcesor>(this, UMiningSkillProcesor::StaticClass(), "Mining Processor");
	UWoodcuttingSkillProcessor* WoodcuttingSkillProcessor = NewObject<UWoodcuttingSkillProcessor>(this, UWoodcuttingSkillProcessor::StaticClass(), "Woodcutting Processor");
	MiningSkillProcessor->ItemCache = ItemCache;
	WoodcuttingSkillProcessor->ItemCache = ItemCache;

	SkillProcessors.Add(EInteractType::MINE, MiningSkillProcessor);
	SkillProcessors.Add(EInteractType::CHOP, WoodcuttingSkillProcessor);
}

void URogueyInteractManager::RemoveActorFromActive(TWeakObjectPtr<ArogueyPawn> Actor)
{
	ActiveInteracts.Remove(Actor);
	if (Actor.IsValid())
	{
		Actor->SetPawnState(EPawnState::IDLE, false);
	}
}

void URogueyInteractManager::RogueyTick(int32 TickIndex)
{
	while (!InteractQueue.IsEmpty())
	{
		FInteractEvent InteractEvent;
		InteractQueue.Dequeue(InteractEvent);

		if (!SkillProcessors.Contains(InteractEvent.InteractType))
		{
			continue;
		}
		ActiveInteracts.Add(InteractEvent.InputActor, InteractEvent);
	}
	
	TSet<TWeakObjectPtr<ArogueyPawn>> FinishedCharacters;
	TArray<FInteractEvent> ActiveEvents;
	
	ActiveInteracts.GenerateValueArray(ActiveEvents);
	for (auto& Event : ActiveEvents)
	{
		bool bFinishInteraction = false;
		TScriptInterface<ISkillProcessor> SkillProcessor = SkillProcessors[Event.InteractType];
		if (Event.TargetObject.IsValid())
		{
			bFinishInteraction = SkillProcessor->ProcessSkillObject(TickIndex, Event.InputActor, Event.TargetObject);
		}
		if (bFinishInteraction)
		{
			FinishedCharacters.Add(Event.InputActor);
		}
	}
	for (auto& Character : FinishedCharacters)
	{
		RemoveActorFromActive(Character);
	}
}

void URogueyInteractManager::EnqueueInteractEvent(FInteractEvent InteractEvent)
{
	InteractQueue.Enqueue(InteractEvent);
}

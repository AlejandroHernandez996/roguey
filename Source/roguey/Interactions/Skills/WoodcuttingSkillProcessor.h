// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillProcessor.h"
#include "UObject/Object.h"
#include "WoodcuttingSkillProcessor.generated.h"

UCLASS()
class ROGUEY_API UWoodcuttingSkillProcessor : public UObject, public ISkillProcessor
{
	GENERATED_BODY()

	virtual const bool ProcessSkillObject(int32 TickIndex, TWeakObjectPtr<ArogueyPawn> SkillActor, TWeakObjectPtr<AArogueyObject> SkillObject) const override;
};

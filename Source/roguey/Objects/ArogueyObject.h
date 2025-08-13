// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Engine/Interactable.h"
#include "Core/Engine/InteractType.h"
#include "UObject/Object.h"
#include "ArogueyObject.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API AArogueyObject : public AActor, public IInteractable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ObjectName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ObjectId;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ExamineText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable", meta = (AllowPrivateAccess = "true"))
	TArray<EInteractType> InteractList {EInteractType::ATTACK};
	virtual const TArray<EInteractType>& GetInteractList() const override
	{
		return InteractList;
	}

	virtual const FString GetRogueyName() const override
	{
		return ObjectName;
	}
	virtual const FString GetExamineText() const override
	{
		return ExamineText;
	}
};

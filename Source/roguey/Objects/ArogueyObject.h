// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyPawn.h"
#include "Items/ItemAtrribute.h"
#include "UObject/Object.h"
#include "ArogueyObject.generated.h"

/**
 * 
 */
UCLASS()
class ROGUEY_API AArogueyObject : public ArogueyPawn
{
	GENERATED_BODY()
public:
	AArogueyObject();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ObjectName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ObjectId;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EItemAttribute RequiredAttribute = EItemAttribute::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RewardResourceId = -1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ResourceSkillingMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredLevel = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ExperienceReward = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDepleted = false;
	UPROPERTY(EditAnywhere, BLueprintReadWrite)
	int32 RespawnTicks = 16;
	UPROPERTY(EditAnywhere, BLueprintReadWrite)
	int32 DepletedOnTick = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DepletedMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ResourceMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResourceHP = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxResourceHP = 3;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;	

	virtual const FString GetRogueyName() const override
	{
		return ObjectName;
	}
	virtual const FString GetExamineText() const override
	{
		return ExamineText;
	}

	void RespawnResource();
};

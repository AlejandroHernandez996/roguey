// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "rogueyItem.h"
#include "Components/BoxComponent.h"
#include "Core/Engine/Interactable.h"
#include "Core/Engine/InteractType.h"
#include "GameFramework/Actor.h"
#include "rogueyItemActor.generated.h"

UCLASS()
class ROGUEY_API ArogueyItemActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ArogueyItemActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FrogueyItem Item;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable", meta = (AllowPrivateAccess = "true"))
	TArray<EInteractType> InteractList {EInteractType::TAKE};

	virtual const TArray<EInteractType>& GetInteractList() const override
	{
		return InteractList;
	}

	virtual const FString GetRogueyName() const override
	{
		return Item.ItemName;
	}
	virtual const FString GetExamineText() const override
	{
		return Item.ItemDescription;
	}
	UFUNCTION(BlueprintCallable)
	FString GetGroundText()
	{
		FString GroundText = Item.ItemName;
		if (Item.Quantity > 1)
		{
			GroundText += " (" + FString::FromInt(Item.Quantity)+ ")";
		}
		return GroundText;
	}
};

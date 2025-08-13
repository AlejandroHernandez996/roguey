// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Engine/rogueyEngine.h"
#include "rogueyGameMode.generated.h"

UCLASS(abstract)
class ArogueyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ArogueyGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UrogueyEngine* Engine;
	UPROPERTY()
	class UrogueyMovementManager* MovementManager;
	UPROPERTY()
	class UrogueyGridManager* GridManager;
	UPROPERTY()
	class UrogueyInputManager* InputManager;
	UPROPERTY()
	class UrogueyCombatManager* CombatManager;
	UPROPERTY()
	class UrogueyInventoryManager* InventoryManager;
	UPROPERTY()
	class UrogueyBehaviorManager* BehaviorManager;
	UPROPERTY()
	class UDeathManager* DeathManager;
	UPROPERTY()
	class USpawnManager* SpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Items")
	class UrogueyItemCache* ItemCache;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	UTexture2D* Texture;
	
	UFUNCTION()
	int32 GetCurrentTick() const
	{
		if (Engine)
		{
			return Engine->GetCurrentTick();
		}
		return -1;
	}
};




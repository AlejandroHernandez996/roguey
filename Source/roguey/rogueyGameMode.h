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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	UTexture2D* Texture;
	
	UFUNCTION()
	uint32 GetCurrentTick() const
	{
		if (Engine)
		{
			return Engine->GetCurrentTick();
		}
		return -1;
	}
};




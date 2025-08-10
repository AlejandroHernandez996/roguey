// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "rogueyPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickEvent, bool, bIsYellow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHoverEvent, const FString&, ActionName, const FString&, ObjectName);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class ArogueyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MouseScrollClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MouseMoveAction;

	UPROPERTY()
	class ArogueyGameMode* RogueyGameMode;

	UPROPERTY(BlueprintAssignable, Category = "PlayerInput")
	FClickEvent OnClickEvent;

	UPROPERTY(BlueprintAssignable, Category = "PlayerInput")
	FHoverEvent OnHoverEvent;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	bool bIsRotating;

public:

	ArogueyPlayerController();

protected:
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	
	void OnInputStarted();
	void OnInputTriggered();
	void OnInputReleased();
	void OnZoomTriggered(const FInputActionInstance& Instance);
	void OnMouseScrollStarted();
	void OnMouseScrollReleased();
	void OnMouseMove(const FInputActionInstance& Instance);
	void DrawHoveredTile(const FVector& HoveredPosition);
};



// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Core/Engine/InteractType.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/rogueyInventory.h"
#include "rogueyPlayerController.generated.h"

struct FrogueyInventory;
struct FInteractTypeArray;
enum class EInteractType : uint8;
class IInteractable;
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickEvent, bool, bIsYellow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatEvent, const FString&, ChatMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHoverEvent, const FString&, ActionName, const FString&, ObjectName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryEvent, const FrogueyInventory, UpdatedInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractMenuEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractInventoryMenuEvent);
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class ArogueyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractMenuAction;

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

	UPROPERTY(BlueprintAssignable, Category = "PlayerInput")
	FInteractMenuEvent OnInteractMenuEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "PlayerInput")
	FInteractInventoryMenuEvent OnInteractInventoryMenuEvent;

	UPROPERTY(BlueprintAssignable, Category = "Examine")
	FOnChatEvent OnChatMessage;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryEvent OnInventoryUpdate;
	
	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	bool bIsRotating;

	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	bool bIsInteractMenuOpen = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	TArray<FInteractTypeArray> InteractMenuEntries;
	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	FVector InteractMenuLocation;
	
public:

	ArogueyPlayerController();

protected:
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	
	void OnInputStarted();
	void OnInputTriggered();
	void OnInputReleased();
	void OnInteractMenuStarted();
	void OnZoomTriggered(const FInputActionInstance& Instance);
	void OnMouseScrollStarted();
	void OnMouseScrollReleased();
	void OnMouseMove(const FInputActionInstance& Instance);
	void DrawHoveredTile(const FVector& HoveredPosition);

	UFUNCTION(BlueprintCallable)
	FString InteractTypeToString(EInteractType InteractType)
	{
		return EInteractTypeToString(InteractType);
	}
	UFUNCTION(BlueprintCallable)
	void InteractMenuInput(AActor* InputActor, EInteractType InteractType);
	
	UFUNCTION(BlueprintCallable)
	void InventoryItemHoverAction(int32 InventoryIndex);
	UFUNCTION(BlueprintCallable)
	void InventoryItemInput(int32 InventoryIndex);
	UFUNCTION(BlueprintCallable)
	void InventoryItemMenuInput(int32 InventoryIndex);

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInput")
	bool bIsUIOnly = false;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/rogueyPawn.h"
#include "Inventory/EquipmentType.h"
#include "rogueyCharacter.generated.h"

class UrogueyInventoryManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExperienceDropEvent, const int32&, ExperienceAmount, ErogueyStatType,
                                             StatType);

UCLASS(abstract)
class ArogueyCharacter : public ArogueyPawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ArogueyCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	void EquipItemMesh(const FrogueyItem EquipItem);

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FExperienceDropEvent OnExperienceDrop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	TMap<EEquipmentType, UStaticMeshComponent*> EquipmentMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	TMap<ErogueyStatType, UAnimMontage*> SkillingMontages;

	virtual EAttackType GetAttackType() override;

	UPROPERTY()
	TWeakObjectPtr<UrogueyInventoryManager> InventoryManager;

	UPROPERTY()
	int32 LastSkillingTick = -1;
};


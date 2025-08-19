// Copyright Epic Games, Inc. All Rights Reserved.

#include "rogueyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Inventory/rogueyInventoryManager.h"

ArogueyCharacter::ArogueyCharacter()
{
	PawnId = -1;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(Mesh);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	for (uint8 i = 0; i < (uint8)EEquipmentType::MAX; i++)
	{
		EEquipmentType EquipType = static_cast<EEquipmentType>(i);
		if (EquipmentHasAMesh(EquipType))
		{
			FString CompName = UEnum::GetValueAsString(EquipType);
			UStaticMeshComponent* EquipComp = CreateDefaultSubobject<UStaticMeshComponent>(*CompName);
			EquipComp->SetupAttachment( Mesh, GetEquipmentSocketName(EquipType));
			EquipmentMap.Add(EquipType, EquipComp);
		}
	}
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ArogueyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ArogueyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ArogueyCharacter::EquipItemMesh(const FrogueyItem EquipItem)
{
	if (EquipmentMap.Contains(EquipItem.EquipmentSlot))
	{
		UStaticMeshComponent* EquipMesh = EquipmentMap[EquipItem.EquipmentSlot];
		EquipMesh->SetStaticMesh(EquipItem.ItemMesh);
		EquipMesh->SetRelativeLocation(EquipItem.EquipMeshRelativeLocation);
		EquipMesh->SetRelativeRotation(EquipItem.EquipMeshRelativeRotation);
		EquipMesh->SetRelativeScale3D(EquipItem.EquipMeshRelativeScale);
	}
}

EAttackType ArogueyCharacter::GetAttackType()
{
	if (InventoryManager.IsValid())
	{
		if (InventoryManager->Equipment.Equipment.Contains(EEquipmentType::WEAPON))
		{
			return InventoryManager->Equipment.Equipment[EEquipmentType::WEAPON].AttackType;
		}
	}
	return AttackType;
}

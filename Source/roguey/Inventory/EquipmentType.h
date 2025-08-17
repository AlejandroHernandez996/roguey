// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	HELMET,
	CHEST,
	LEGS,
	GLOVES,
	BOOTS,
	CAPE,
	WEAPON,
	SHIELD,
	RING,
	AMULET,
	NONE,
	MAX UMETA(Hidden)
};
FORCEINLINE FName GetEquipmentSocketName(EEquipmentType Type)
{
	switch (Type)
	{
	case EEquipmentType::HELMET:  return "head";
	case EEquipmentType::CAPE:    return "neck_01";
	case EEquipmentType::WEAPON:  return "hand_r";
	case EEquipmentType::SHIELD:  return "hand_l";
	case EEquipmentType::AMULET:  return "neck_01";
	default: return NAME_None;
	}
}
FORCEINLINE bool EquipmentHasAMesh(EEquipmentType Type)
{
	switch (Type)
	{
	case EEquipmentType::HELMET:
	case EEquipmentType::CAPE:  
	case EEquipmentType::WEAPON:
	case EEquipmentType::SHIELD:
	case EEquipmentType::AMULET:
		return true;
	default: return false;
	}
}
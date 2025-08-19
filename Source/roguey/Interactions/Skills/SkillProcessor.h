#pragma once
#include "Characters/Player/rogueyCharacter.h"
#include "Inventory/rogueyInventoryManager.h"
#include "Items/ItemAtrribute.h"
#include "Objects/ArogueyObject.h"

#include "SkillProcessor.generated.h"

class UrogueyItemCache;
class AArogueyObject;
class ArogueyPawn;
enum class EInteractType : uint8;

UINTERFACE(BlueprintType, Blueprintable)
class USkillProcessor : public UInterface
{
	GENERATED_BODY()
};

class ROGUEY_API ISkillProcessor
{
	GENERATED_BODY()
public:
	virtual const bool ProcessSkillObject(int32 TickIndex, TWeakObjectPtr<ArogueyPawn> SkillActor, TWeakObjectPtr<AArogueyObject> SkillObject) const = 0;
	static bool HasValidSkillingTool(TWeakObjectPtr<ArogueyPawn> SkillActor,
	                                 TWeakObjectPtr<AArogueyObject> SkillObject);
	static bool HasValidLevel(TWeakObjectPtr<ArogueyPawn> SkillActor, TWeakObjectPtr<AArogueyObject> SkillObject,
	                          ErogueyStatType StatType);

	TObjectPtr<UrogueyItemCache> ItemCache = nullptr;
};

inline bool ISkillProcessor::HasValidSkillingTool(TWeakObjectPtr<ArogueyPawn> SkillActor,
                                                  TWeakObjectPtr<AArogueyObject> SkillObject)
{
	if (SkillObject->RequiredAttribute == EItemAttribute::NONE)
	{
		return true;
	}
	ArogueyCharacter* Character = Cast<ArogueyCharacter>(SkillActor);
	TSet<EItemAttribute> EquippedAttributes;
	if (Character->InventoryManager->Equipment.Equipment.Contains(EEquipmentType::WEAPON))
	{
		EquippedAttributes = Character->InventoryManager->Equipment.Equipment[EEquipmentType::WEAPON].ItemAttributes;
	}
		
	return EquippedAttributes.Contains(SkillObject->RequiredAttribute);
}

inline bool ISkillProcessor::HasValidLevel(TWeakObjectPtr<ArogueyPawn> SkillActor,
                                           TWeakObjectPtr<AArogueyObject> SkillObject, ErogueyStatType StatType)
{
	ArogueyCharacter* Character = Cast<ArogueyCharacter>(SkillActor);
	int32 RequiredLevel = SkillObject->RequiredLevel;

	int32 CurrentLevel = Character->StatPage.StatPage[StatType].CurrentLevel = RequiredLevel;
		
	return CurrentLevel >= RequiredLevel;
}

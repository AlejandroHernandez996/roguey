// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodcuttingSkillProcessor.h"

#include "Characters/Player/Controller/rogueyPlayerController.h"
#include "Characters/PawnState.h"
#include "Items/rogueyItemCache.h"

const bool UWoodcuttingSkillProcessor::ProcessSkillObject(int32 TickIndex, TWeakObjectPtr<ArogueyPawn> SkillActor,
                                                          TWeakObjectPtr<AArogueyObject> SkillObject) const
{
	SkillActor->RotateAtPawn(Cast<ArogueyPawn>(SkillObject));
	if (!HasValidSkillingTool(SkillActor, SkillObject))
	{
		Cast<ArogueyPlayerController>(SkillActor->GetController())->OnChatMessage.Broadcast("You require an axe!");
		return true;
	}
	if (!HasValidLevel(SkillActor, SkillObject, ErogueyStatType::WOODCUTTING))
	{
		Cast<ArogueyPlayerController>(SkillActor->GetController())->OnChatMessage.Broadcast("You do not meet required woodcutting level!");
		return true;
	}
	if (SkillObject->bIsDepleted)
	{
		Cast<ArogueyPlayerController>(SkillActor->GetController())->OnChatMessage.Broadcast("Tree is currently depleted.");
		return true;
	}
	
	ArogueyCharacter* Character = Cast<ArogueyCharacter>(SkillActor);
	Character->SetPawnState(EPawnState::SKILLING, false);
	FrogueyItem Axe = Character->InventoryManager->Equipment.Equipment[EEquipmentType::WEAPON];
	int32 AxeTickSpeed = Axe.ItemStatPage.StatPage[EItemStatType::ATTACK_SPEED].BaseStat;
	int32 LastSkillingTick = Character->LastSkillingTick;
	
	if (TickIndex - LastSkillingTick >= AxeTickSpeed)
	{
		Character->LastSkillingTick = TickIndex;

		if (Character->SkillingMontages.Contains(ErogueyStatType::WOODCUTTING))
		{
			Character->PlayAnimMontage(Character->SkillingMontages[ErogueyStatType::WOODCUTTING]);
		}
		
		int32 PlayerLevel = Character->StatPage.StatPage[ErogueyStatType::WOODCUTTING].CurrentLevel;
		int32 ToolBonus   = Axe.ItemStatPage.StatPage[EItemStatType::SKILL_BONUS].BaseStat;
		float ResourceDifficulty = SkillObject->RequiredLevel * SkillObject->ResourceSkillingMultiplier;

		float SuccessChance = (PlayerLevel + ToolBonus) / (ResourceDifficulty + 1.0f);
		SuccessChance = FMath::Clamp(SuccessChance, 0.05f, 0.95f); 

		if (FMath::FRand() < SuccessChance)
		{
			FrogueyItem RewardItem = ItemCache->Items[SkillObject->RewardResourceId];
			Character->InventoryManager->Inventory.AddItem(RewardItem);
			Cast<ArogueyPlayerController>(Character->GetController())->OnInventoryUpdate.Broadcast(Character->InventoryManager->Inventory);
			Character->StatPage.StatPage[ErogueyStatType::WOODCUTTING].IncrementExperience(SkillObject->ExperienceReward);
			Character->OnExperienceDrop.Broadcast(SkillObject->ExperienceReward, ErogueyStatType::WOODCUTTING);
			Cast<ArogueyPlayerController>(Character->GetController())->OnChatMessage.Broadcast(
				FString::Printf(TEXT("You chop some %s."), * ItemCache->Items[SkillObject->RewardResourceId].ItemName)
			);
			
			SkillObject->ResourceHP--;
			if (SkillObject->ResourceHP <= 0)
			{
				SkillObject->bIsDepleted = true;
				SkillObject->DepletedOnTick = TickIndex;

				if (SkillObject->ResourceMesh) SkillObject->ResourceMesh->SetVisibility(false);
				if (SkillObject->DepletedMesh) SkillObject->DepletedMesh->SetVisibility(true);
				return true;
			}
		}
		else
		{
			Cast<ArogueyPlayerController>(Character->GetController())->OnChatMessage.Broadcast("You fail to chop the tree.");
		}
	}
	return false;
}

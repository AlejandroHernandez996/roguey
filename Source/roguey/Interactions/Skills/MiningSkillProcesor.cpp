
#include "MiningSkillProcesor.h"

#include "Characters/Player/Controller/rogueyPlayerController.h"
#include "Objects/ArogueyObject.h"

const bool UMiningSkillProcesor::ProcessSkillObject(int32 TickIndex,
                                                    TWeakObjectPtr<ArogueyPawn> SkillActor, TWeakObjectPtr<AArogueyObject> SkillObject) const
{
	if (!HasValidSkillingTool(SkillActor, SkillObject))
	{
		Cast<ArogueyPlayerController>(SkillActor->GetController())->OnChatMessage.Broadcast("You require a pickaxe!");
		return true;
	}

	return false;
}

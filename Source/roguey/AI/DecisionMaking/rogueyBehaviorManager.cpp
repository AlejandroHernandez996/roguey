// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyBehaviorManager.h"

#include "Characters/rogueyPawn.h"
#include "Grid/rogueyGridManager.h"
#include "Input/Input.h"
#include "Input/InputType.h"
#include "Input/rogueyInputManager.h"

void UrogueyBehaviorManager::RogueyTick(int32 TickIndex)
{
	TArray<TWeakObjectPtr<ArogueyPawn>> rogueyPawns;
	GridManager->Grid.ActorMapLocation.GetKeys(rogueyPawns);
	ArogueyPawn* PlayerPawn = Cast<ArogueyPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	for (TWeakObjectPtr<ArogueyPawn> Pawn : rogueyPawns)
	{
		if (Pawn->PawnState != EPawnState::IDLE) continue;
		FBehavior Behavior = Pawn->Behavior;
		switch (Behavior.BehaviorType)
		{
		case EBehaviorType::AGGRESSIVE:
			if (!Pawn->TargetPawn.IsValid() && GridManager->IsPawnInAggroRange(Pawn, PlayerPawn))
			{
				FInput Input(TickIndex, EInputType::ATTACK, PlayerPawn->GetActorLocation(), Pawn.Get());
				Input.TargetPawn = PlayerPawn;
				InputManager->EnqueueInput(Input);
			}else
			{
				bool bShouldRoam = FMath::RandRange(1,7) == 1;
				if (bShouldRoam)
				{
					FIntVector2 RoamTile = GridManager->FindRandomTileInRangeOfPawn(Pawn, 5);
					FInput Input(TickIndex, EInputType::MOVE, GridUtils::GridToWorld(RoamTile), Pawn);
					InputManager->EnqueueInput(Input);
				}
			}
			break;
		case EBehaviorType::PASSIVE:
			if (!Pawn->ThreatList.IsEmpty())
			{
				FInput Input(TickIndex, EInputType::ATTACK, PlayerPawn->GetActorLocation(), Pawn.Get());
				Input.TargetPawn = Pawn->ThreatList.Array()[0].Get();
				InputManager->EnqueueInput(Input);
			}
			break;
		case EBehaviorType::PACIFIST:
			break;
		case EBehaviorType::NONE:
			break;
		}
	}
}

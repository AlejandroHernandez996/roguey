#pragma once

#include "CoreMinimal.h"
#include "RogueyProjectileEvent.generated.h"

class ArogueyProjectile;
class ArogueyPawn;

USTRUCT(BlueprintType)
struct FRogueyProjectileEvent
{
	GENERATED_BODY()
	
	int32 Tick;
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> FromActor;
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> ToActor;
	UPROPERTY()
	TSubclassOf<ArogueyProjectile> ProjectileClass;
	UPROPERTY()
	int32 Damage;

	FRogueyProjectileEvent(): Tick(0), Damage(0)
	{
	}

	FRogueyProjectileEvent(
		TWeakObjectPtr<ArogueyPawn> InFromActor,
		TWeakObjectPtr<ArogueyPawn> InToActor,
		int32 InTick,
		TSubclassOf<ArogueyProjectile> InProjectileClass,
		int32 InDamage)
		: Tick(InTick)
		  , FromActor(InFromActor)
		  , ToActor(InToActor)
		  , ProjectileClass(InProjectileClass)
	      , Damage(InDamage)
	{
	}
};
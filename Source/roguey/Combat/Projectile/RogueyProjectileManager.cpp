// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueyProjectileManager.h"

#include "rogueyProjectile.h"
#include "RogueyProjectileEvent.h"
#include "Characters/rogueyPawn.h"
#include "Grid/rogueyGridManager.h"
#include "Grid/Util/GridUtils.h"

void URogueyProjectileManager::RogueyTick(int32 TickIndex)
{
	while (!ProjectileQueue.IsEmpty())
	{
		FRogueyProjectileEvent Event;
		ProjectileQueue.Dequeue(Event);

		ArogueyProjectile* Projectile = CreateProjectile(Event);
		if (Projectile)
		{
			ActiveProjectiles.Add(Projectile);
		}
	}
	ProcessActiveProjectiles(TickIndex);
}

void URogueyProjectileManager::ProcessActiveProjectiles(int32 TickIndex)
{
	TSet<TWeakObjectPtr<ArogueyProjectile>> ProjectilesToDestroy;
	for (auto& Projectile : ActiveProjectiles)
	{
		if (!Projectile.IsValid())
		{
			ProjectilesToDestroy.Add(Projectile);
		}
		else if (TickIndex >= Projectile->TravelTimeInTicks + Projectile->SpawnTick)
		{
			Projectile->ProcessHit();
			ProjectilesToDestroy.Add(Projectile);
		}
	}

	for (auto& Projectile : ProjectilesToDestroy)
	{
		ActiveProjectiles.Remove(Projectile);
		Projectile->Destroy();
	}
}

ArogueyProjectile* URogueyProjectileManager::CreateProjectile(FRogueyProjectileEvent Event)
{
	if (!Event.ProjectileClass)
	{
		return nullptr;
	}

	TWeakObjectPtr<ArogueyPawn> ToActor = Event.ToActor;
	TWeakObjectPtr<ArogueyPawn> FromActor = Event.FromActor;

	if (!ToActor.IsValid() || !FromActor.IsValid())
	{
		return nullptr;
	}
	UWorld* World = FromActor->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FVector SpawnLocation = FromActor->GetActorLocation();
	FRotator SpawnRotation = (ToActor->GetActorLocation() - SpawnLocation).Rotation();

	FActorSpawnParameters Params;
	Params.Owner = FromActor.Get();
	Params.Instigator  = FromActor.Get();
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArogueyProjectile* Projectile =
		World->SpawnActor<ArogueyProjectile>(
			Event.ProjectileClass,
			SpawnLocation,
			SpawnRotation,
			Params);

	if (!Projectile)
	{
		return nullptr;
	}

	Projectile->OwningPawn = FromActor;
	Projectile->TargetPawn = ToActor;
	Projectile->ProjectileDamage = Event.Damage;
	Projectile->SpawnTick = Event.Tick;
	Projectile->TileDistance = GridUtils::GridDistance(GridUtils::WorldToGrid(FromActor->GetActorLocation()), GridUtils::WorldToGrid(ToActor->GetActorLocation()));
	Projectile->InitializeMovement();
	Projectile->FaceTarget();
	return Projectile;
}

void URogueyProjectileManager::EnqueueProjectileEvent(FRogueyProjectileEvent Event)
{
	ProjectileQueue.Enqueue(Event);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyProjectile.h"

#include "Characters/rogueyPawn.h"
#include "Kismet/KismetMathLibrary.h"

ArogueyProjectile::ArogueyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ArogueyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ArogueyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPawn.IsValid())
	{
		FaceTarget();
		MoveToTarget(DeltaTime);
	}else
	{
		SetHidden(true);
	}
}

void ArogueyProjectile::InitializeMovement()
{
	const FVector StartLocation = GetActorLocation();
	const float Distance = FVector::Dist(StartLocation, TargetPawn->GetActorLocation());

	float TimeToDestination = TravelTimeInTicks < 2 ? .6f : (TravelTimeInTicks)*.6f;
	ProjectileSpeed = Distance / TimeToDestination;

}

void ArogueyProjectile::MoveToTarget(float DeltaTime)
{
	if (!TargetPawn.IsValid())
	{
		Destroy();
		return;
	}

	const FVector StartLocation = GetActorLocation();
	const FVector TargetLocation = TargetPawn->GetActorLocation(); 

	FVector Direction = TargetLocation - StartLocation;
	Direction.Normalize(); 

	FVector MovementThisFrame = Direction * ProjectileSpeed * DeltaTime;

	FVector NewLocation = StartLocation + MovementThisFrame;
	SetActorLocation(NewLocation);

	SetActorRotation(Direction.Rotation());
}

void ArogueyProjectile::FaceTarget()
{
	if (!TargetPawn.IsValid())
	{
		return;
	}
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		TargetPawn->GetActorLocation()
	);
	SetActorRotation(LookAtRotation);
}

void ArogueyProjectile::ProcessHit()
{
	if (!TargetPawn.IsValid())
	{
		SetHidden(true);
		return;
	}
	
	TargetPawn->UpdateCurrentStat(ErogueyStatType::HEALTH, -ProjectileDamage);

	SetHidden(true);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyPawn.h"

#include "PawnState.h"
#include "rogueyGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Grid/rogueyGridManager.h"
#include "Grid/Util/GridUtils.h"
#include "Items/rogueyItemCache.h"
#include "Player/rogueyCharacter.h"

FName ArogueyPawn::MeshComponentName(TEXT("CharacterMesh0"));
FName ArogueyPawn::CollisionComponentName(TEXT("PawnCollision"));

ArogueyPawn::ArogueyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(CollisionComponentName);
	CollisionComponent->InitCapsuleSize(42.f, 96.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(MeshComponentName);
	Mesh->SetupAttachment(CollisionComponent);
	if (Mesh)
	{
		Mesh->SetupAttachment(CollisionComponent); 
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}
}

void ArogueyPawn::BeginPlay()
{
	Super::BeginPlay();
	ArogueyGameMode* RogueyGameMode = Cast<ArogueyGameMode>(GetWorld()->GetAuthGameMode());
	RogueyGameMode->GridManager->AddActorToGrid(this,GridUtils::WorldToGrid(this->GetActorLocation()));
	RogueyGameMode->ItemCache->InitLootTable(this);
	if (ArogueyCharacter* PlayerCharacter = Cast<ArogueyCharacter>(this))
	{
		RogueyGameMode->GridManager->PlayerCharacter = PlayerCharacter;
	}
	TrueTileQueue.Enqueue({GridUtils::WorldToGrid(this->GetActorLocation()), 1.0f});
	SetPawnState(EPawnState::IDLE, false);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ArogueyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetPawn.Get() && TargetPawn->PawnState == EPawnState::DEAD)
	{
		TargetPawn = nullptr;
	}
	if (PawnState == EPawnState::DEAD)
	{
		return;
	}
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	if (bHit)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = HitResult.Location.Z + CollisionComponent->GetScaledCapsuleHalfHeight();
		SetActorLocation(NewLocation);
	}

	ArogueyGameMode* RogueyGameMode = Cast<ArogueyGameMode>(GetWorld()->GetAuthGameMode());
	if (RogueyGameMode && RogueyGameMode->GridManager->Grid.ActorMapLocation.Contains(this))
	{
		DrawTrueTile(RogueyGameMode->GridManager->Grid.ActorMapLocation[this],DeltaTime*2.0f);
	}

	if (TrueTileQueue.IsEmpty())
	{
		if (PawnState != EPawnState::ATTACKING)
		{
			SetPawnState(EPawnState::IDLE, false);
		}
		return;
	}
	TPair<FIntVector2, float> TargetTrueTile = *TrueTileQueue.Peek();
	FVector TargetLocation = GridUtils::GridToWorld(TargetTrueTile.Key);
	FVector CurrentLocation = GetActorLocation();

	TargetLocation.Z = CurrentLocation.Z;

	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	
	float MovementSpeed = BaseSpeed * TargetTrueTile.Value / 0.6f;
	UE_LOG(LogTemp, Log, TEXT("Distance: %f") ,TargetTrueTile.Value);

	FVector MovementStep = Direction * MovementSpeed * DeltaTime;

	if (FVector::Dist(CurrentLocation, TargetLocation) > 5.0f)
	{
		SetActorLocation(CurrentLocation + MovementStep);
		bool bPrevIsWalking = bIsWalking;
		bIsWalking = TargetTrueTile.Value < 2.0f;
		SetPawnState(EPawnState::MOVING, bPrevIsWalking != bIsWalking);
		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
	else
	{
		SetActorLocation(TargetLocation);
		if (!TrueTileQueue.IsEmpty())
		{
			TrueTileQueue.Pop();
			QueueSize--;
		}
	}
}

void ArogueyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ArogueyPawn::ClearTarget()
{
	TargetPawn.Reset();
	OnTargetPawn.Broadcast(nullptr);
}

void ArogueyPawn::SetTarget(TWeakObjectPtr<ArogueyPawn> Target)
{
	if (Target.IsValid())
	{
		TargetPawn = Target;
		TargetPawn->ThreatList.Add(this);
		OnTargetPawn.Broadcast(Target.Get());
	}
}

float ArogueyPawn::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	UAnimInstance * AnimInstance = (Mesh)? Mesh->GetAnimInstance() : nullptr; 
	if( AnimMontage && AnimInstance )
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		if (Duration > 0.f)
		{
			if( StartSectionName != NAME_None )
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}	

	return 0.f;
}

void ArogueyPawn::RotateAtPawn(ArogueyPawn* ToActor)
{
	if (ToActor)
	{
		FVector TargetLocation = ToActor->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();

		TargetLocation.Z = CurrentLocation.Z;

		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
}

void ArogueyPawn::UpdateCurrentStat(ErogueyStatType StatType, int32 DeltaValue)
{
	StatPage.StatPage[StatType].CurrentLevel += DeltaValue;

	if (StatType == ErogueyStatType::HEALTH)
	{
		if (DeltaValue == 0)
		{
			OnBlockEvent.Broadcast(this);
		}
		if (DeltaValue < 0)
		{
			OnDamageEvent.Broadcast(FMath::Abs(DeltaValue), this);
		}
	}
}

void ArogueyPawn::DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime)
{
	FVector WorldTileCenter = GridUtils::GridToWorld(TrueTileLocation);

	float HalfTileSize = 50.0f; 
	float ZOffset = 1.0f;      

	FVector CornerOffsets[] = {
		FVector(HalfTileSize, HalfTileSize, 0),    // Top Right
		FVector(-HalfTileSize, HalfTileSize, 0),   // Top Left
		FVector(HalfTileSize, -HalfTileSize, 0),   // Bottom Right
		FVector(-HalfTileSize, -HalfTileSize, 0)   // Bottom Left
	};

	FVector CornerHeights[4];

	for (int32 i = 0; i < 4; i++)
	{
		FVector CornerLocation = WorldTileCenter + CornerOffsets[i];
		FVector StartLocation = CornerLocation + FVector(0, 0, 1000);
		FVector EndLocation = CornerLocation - FVector(0, 0, 2000); 

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility);

		if (bHit)
		{
			CornerHeights[i] = HitResult.Location + FVector(0, 0, ZOffset);
		}
		else
		{
			CornerHeights[i] = CornerLocation + FVector(0, 0, ZOffset);
		}
	}

	DrawDebugLine(GetWorld(), CornerHeights[0], CornerHeights[1], TrueTileColor, false, DecayTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[1], CornerHeights[3], TrueTileColor, false, DecayTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[3], CornerHeights[2], TrueTileColor, false, DecayTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), CornerHeights[2], CornerHeights[0], TrueTileColor, false, DecayTime, 0, 2.0f);
}

void ArogueyPawn::SetPawnState(EPawnState State, bool bOverride = false)
{
	if (State == PawnState && !bOverride) return;
	PawnState = State;
	switch (State)
	{
	case EPawnState::IDLE:
		PlayAnimMontage(IdleMontage);
		break;
	case EPawnState::MOVING:
	case EPawnState::FOLLOWING:
	case EPawnState::ATTACK_MOVING:
		if (bIsWalking)
		{
			PlayAnimMontage(WalkMontage);
		}else
		{
			PlayAnimMontage(RunMontage);
		}
		break;
	case EPawnState::ATTACKING:
		if (DefaultAttack)
		{
			PlayAnimMontage(DefaultAttack);
		}
	case EPawnState::DEAD:
		PlayAnimMontage(DeathAnim);
		break;
	}
}
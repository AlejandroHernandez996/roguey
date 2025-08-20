// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnState.h"
#include "AI/DecisionMaking/Behavior.h"
#include "Combat/AttackType.h"
#include "Core/Engine/Interactable.h"
#include "Core/Engine/InteractType.h"
#include "Enemies/rogueyLootTable.h"
#include "GameFramework/Pawn.h"
#include "Stats/rogueyStatPage.h"
#include "rogueyPawn.generated.h"

enum class EAttackType : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatDamageEvent, const int32&, DamageAmount, ArogueyPawn*, OwningPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatBlockEvent, ArogueyPawn*, OwningPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetPawnEvent, ArogueyPawn*, CurrentTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnDeath, int32, TickIndex);

UCLASS()
class ROGUEY_API ArogueyPawn : public APawn, public IInteractable
{
	GENERATED_BODY()

public:
	static FName MeshComponentName;
	static FName CollisionComponentName;
	ArogueyPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ClearTarget();
	void SetTarget(TWeakObjectPtr<ArogueyPawn> Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NpcId = -1;
	
	UPROPERTY(BlueprintAssignable, Category = "Target")
	FTargetPawnEvent OnTargetPawn;

	UPROPERTY(EditAnywhere)
	int32 PawnId = 0;
	bool bIsAnimLocked = false;

	UFUNCTION(BlueprintCallable, Category=Animation)
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	void RotateAtPawn(ArogueyPawn* ToActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FrogueyStatPage StatPage;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FCombatDamageEvent OnDamageEvent;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FCombatBlockEvent OnBlockEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 LastAttackTickIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 DefaultAttackCooldown = 3;
	int32 LastAttackCooldown = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 TileMoveSpeed = 1;
	UFUNCTION()
	void UpdateCurrentStat(ErogueyStatType StatType, int32 DeltaValue);

	UFUNCTION()
	void DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime);

	UPROPERTY()
	EPawnState PawnState = EPawnState::SPAWNED;
	UPROPERTY()
	bool bIsWalking;
	
	UFUNCTION()
	void SetPawnState(EPawnState State, bool bOverride);

	TQueue<TPair<FIntVector2,float>> TrueTileQueue;
	int32 QueueSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* WalkMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* RunMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* IdleMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DefaultAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString RogueyName = "NA";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	FBehavior Behavior;
	UPROPERTY()
	TWeakObjectPtr<ArogueyPawn> TargetPawn;
	UPROPERTY()
	TSet<TWeakObjectPtr<ArogueyPawn>> ThreatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FColor TrueTileColor = FColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 GridSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int32 TeamId = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	FrogueyLootTable LootTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactable", meta = (AllowPrivateAccess = "true"))
	TArray<EInteractType> InteractList {EInteractType::ATTACK};
	UPROPERTY(EditAnywhere,	BlueprintReadWrite,	Category="Flavor")
	FString ExamineText = "Did you forget?";
	virtual const TArray<EInteractType>& GetInteractList() const override
	{
		return InteractList;
	}
	virtual const FString GetRogueyName() const override
	{
		return RogueyName;
	}
	virtual const FString GetExamineText() const override
	{
		return ExamineText;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAttackType AttackType = EAttackType::MELEE;

	virtual EAttackType GetAttackType();

	UPROPERTY(BlueprintAssignable)
	FOnPawnDeath OnPawnDeath;

	void Die(int32 TickIndex)
	{
		OnPawnDeath.Broadcast(TickIndex);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ArogueyProjectile> ProjectileClass;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnState.h"
#include "AI/DecisionMaking/Behavior.h"
#include "Core/Engine/Interactable.h"
#include "Core/Engine/InteractType.h"
#include "Enemies/rogueyLootTable.h"
#include "GameFramework/Pawn.h"
#include "Stats/rogueyStatPage.h"
#include "rogueyPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatDamageEvent, const int32&, DamageAmount, ArogueyPawn*, OwningPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatBlockEvent, ArogueyPawn*, OwningPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetPawnEvent, ArogueyPawn*, CurrentTarget);

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
	void SetTarget(ArogueyPawn* Target);
	UPROPERTY(BlueprintAssignable, Category = "Target")
	FTargetPawnEvent OnTargetPawn;

	UPROPERTY(EditAnywhere)
	int32 PawnId = 0;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 TileMoveSpeed = 1;
	UFUNCTION()
	void UpdateCurrentStat(ErogueyStatType StatType, int32 DeltaValue);

	UFUNCTION()
	void DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime);

	UPROPERTY()
	EPawnState PawnState = EPawnState::IDLE;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString RogueyName = "NA";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	FBehavior Behavior;
	UPROPERTY()
	ArogueyPawn* TargetPawn;
	UPROPERTY()
	TSet<ArogueyPawn*> ThreatList;
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

};

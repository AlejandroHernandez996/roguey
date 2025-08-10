// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnState.h"
#include "GameFramework/Pawn.h"
#include "Stats/rogueyStatPage.h"
#include "rogueyPawn.generated.h"

UCLASS()
class ROGUEY_API ArogueyPawn : public APawn
{
	GENERATED_BODY()

public:
	static FName MeshComponentName;
	static FName CollisionComponentName;
	// Sets default values for this pawn's properties
	ArogueyPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category=Animation)
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FrogueyStatPage StatPage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 CurrentAttackCooldownInTicks = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 DefaultAttackCooldown = 3;

	UFUNCTION()
	void DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime);

	UPROPERTY()
	EPawnState PawnState = EPawnState::IDLE;
	UPROPERTY()
	bool bIsWalking;
	
	UFUNCTION()
	void SetPawnState(EPawnState State);

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
};

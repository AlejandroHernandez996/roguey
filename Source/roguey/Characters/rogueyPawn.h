// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnState.h"
#include "GameFramework/Pawn.h"
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Play Animation Montage on the character mesh. Returns the length of the animation montage in seconds, or 0.f if failed to play. **/
	UFUNCTION(BlueprintCallable, Category=Animation)
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UFUNCTION()
	void DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime);

	UPROPERTY()
	EPawnState PawnState = EPawnState::IDLE;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* CollisionComponent;
};

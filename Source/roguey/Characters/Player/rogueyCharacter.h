// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PawnState.h"
#include "GameFramework/Character.h"
#include "rogueyCharacter.generated.h"

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class ArogueyCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	ArogueyCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	TQueue<TPair<FIntVector2,float>> TrueTileQueue;
	int32 QueueSize = 0;

	UFUNCTION()
	void DrawTrueTile(FIntVector2 TrueTileLocation, float DecayTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* WalkMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* RunMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* IdleMontage;

	UPROPERTY()
	EPawnState PawnState = EPawnState::IDLE;

	UFUNCTION()
	void SetPawnState(EPawnState State);
	UFUNCTION()
	void PlayRogueyAnimMontage(UAnimMontage* AnimMontage);
};


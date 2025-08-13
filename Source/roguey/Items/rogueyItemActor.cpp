// Fill out your copyright notice in the Description page of Project Settings.


#include "rogueyItemActor.h"
#include "Components/BoxComponent.h"


ArogueyItemActor::ArogueyItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Item Hitbox"));
	CollisionComponent->InitBoxExtent(FVector(20.0f, 20.0f, 20.0f));
	CollisionComponent->SetCollisionProfileName(TEXT("Item"));
	RootComponent = CollisionComponent; // Root is now the collider

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
}

// Called when the game starts or when spawned
void ArogueyItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ArogueyItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		FVector NewLocation = HitResult.Location;
		NewLocation.Z += CollisionComponent->GetScaledBoxExtent().Z/2;
		SetActorLocation(NewLocation);
	}
}


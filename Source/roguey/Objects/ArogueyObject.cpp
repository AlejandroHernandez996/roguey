// Fill out your copyright notice in the Description page of Project Settings.


#include "ArogueyObject.h"

#include "Components/CapsuleComponent.h"

AArogueyObject::AArogueyObject()
{
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	DepletedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DepletedMesh"));
	DepletedMesh->SetupAttachment(CollisionComponent);
	ResourceMesh->SetupAttachment(CollisionComponent);

	DepletedMesh->SetVisibility(false);
}

void AArogueyObject::Tick(float DeltaSeconds)
{
}

void AArogueyObject::BeginPlay()
{
}

void AArogueyObject::RespawnResource()
{
	bIsDepleted = false;
	ResourceHP = MaxResourceHP;

	if (ResourceMesh) ResourceMesh->SetVisibility(true);
	if (DepletedMesh) DepletedMesh->SetVisibility(false);
}

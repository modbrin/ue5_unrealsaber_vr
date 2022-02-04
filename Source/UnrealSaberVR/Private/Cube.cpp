// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

// Sets default values
ACube::ACube(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));
	CubeMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeMesh"));
	CubeMeshCounterpart = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeMeshCounterPart"));

	CubeMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	CubeMeshCounterpart->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	CubeMeshCounterpart->SetVisibility(false);
	SetRootComponent(SceneRoot);
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	CubeMesh->SetSimulatePhysics(false);
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformMovement(DeltaTime);
}

void ACube::PerformMovement(float DeltaTime)
{
	if (CubeMesh != nullptr && bIsFlying)
	{
		SetActorLocation(GetActorLocation() + WorldMovementDirection.GetSafeNormal() * MovementSpeed * DeltaTime);
	}
}

void ACube::Hit(FVector HitLocation, FVector HitNormal, FVector SaberDirection)
{
	bIsFlying = false;
}


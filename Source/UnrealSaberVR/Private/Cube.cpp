// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

#include "SaberUtils.h"

// Sets default values
ACube::ACube(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));
	CubeMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeMesh"));
	CubeCounterpartMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeMeshCounterPart"));

	CubeMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	CubeCounterpartMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	CubeCounterpartMesh->SetVisibility(false);
	SetRootComponent(SceneRoot);
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	CubeMesh->SetSimulatePhysics(false);

	if (CubeDynamicMaterial != nullptr)
	{
		CubeDynamicMaterial->SetScalarParameterValue(TEXT("IsCutEnabled"), 0.f);
		CubeCounterpartDynamicMaterial->SetScalarParameterValue(TEXT("IsCutEnabled"), 0.f);
	}
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

void ACube::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (CubeDynamicMaterial == nullptr && CubeMesh != nullptr)
	{
		CubeDynamicMaterial = UMaterialInstanceDynamic::Create(CubeMesh->GetMaterial(0), nullptr);
		CubeMesh->SetMaterial(0, CubeDynamicMaterial);
	}
	if (CubeCounterpartDynamicMaterial == nullptr && CubeCounterpartMesh != nullptr)
	{
		CubeCounterpartDynamicMaterial = UMaterialInstanceDynamic::Create(CubeCounterpartMesh->GetMaterial(0), nullptr);
		CubeCounterpartMesh->SetMaterial(0, CubeCounterpartDynamicMaterial);
	}
}

void ACube::CutOccured(FVector CutterPlanePoint, FVector CutterPlaneNormal)
{
	FVector BoxCenter = GetActorLocation();
	FVector CutterOffsetVector = SaberUtils::GetOrthogonalVectorPointToPlane(BoxCenter, CutterPlanePoint, CutterPlaneNormal);
	CutterOffsetVector = GetActorRotation().UnrotateVector(CutterOffsetVector);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalX"), CutterOffsetVector.X);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalY"), CutterOffsetVector.Y);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalZ"), CutterOffsetVector.Z);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutOffset"), CutterOffsetVector.Size());
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("IsCutEnabled"), 1.f);
}

FVector ACube::GetMovementDirection()
{
	return WorldMovementDirection;
}


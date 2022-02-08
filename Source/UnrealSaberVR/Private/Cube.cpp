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

void ACube::OnDestructionTimerEnd()
{
	GetWorldTimerManager().ClearTimer(DestroyCountdownTimer);
	Destroy();
}

void ACube::CutOccured(FVector CutterPlanePoint, FVector CutterPlaneNormal)
{
	// Calculate material cut params from cutter plane
	FVector BoxCenter = GetActorLocation();
	FVector CutterOffsetVector = SaberUtils::GetOrthogonalVectorPointToPlane(BoxCenter, CutterPlanePoint, CutterPlaneNormal);
	FVector AdjustedCutterOffsetVector = GetActorRotation().UnrotateVector(CutterOffsetVector);

	// Apply material to main fragment
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalX"), AdjustedCutterOffsetVector.X);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalY"), AdjustedCutterOffsetVector.Y);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalZ"), AdjustedCutterOffsetVector.Z);
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("CutOffset"), AdjustedCutterOffsetVector.Size());
	CubeDynamicMaterial->SetScalarParameterValue(TEXT("IsCutEnabled"), 1.f);
	CubeMesh->SetSimulatePhysics(true);
	CubeMesh->AddImpulse(CutterOffsetVector.GetSafeNormal() * SideImpulseOnCut);

	// Apply material to counterpart fragment
	CubeCounterpartDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalX"), -AdjustedCutterOffsetVector.X);
	CubeCounterpartDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalY"), -AdjustedCutterOffsetVector.Y);
	CubeCounterpartDynamicMaterial->SetScalarParameterValue(TEXT("CutNormalZ"), -AdjustedCutterOffsetVector.Z);
	CubeCounterpartDynamicMaterial->SetScalarParameterValue(TEXT("CutOffset"), -AdjustedCutterOffsetVector.Size());
	CubeCounterpartDynamicMaterial->SetScalarParameterValue(TEXT("IsCutEnabled"), 1.f);
	CubeCounterpartMesh->SetVisibility(true);
	CubeCounterpartMesh->SetSimulatePhysics(true);
	CubeCounterpartMesh->AddImpulse(-CutterOffsetVector.GetSafeNormal() * SideImpulseOnCut);

	// Start countdown until self-destruction
	GetWorldTimerManager().SetTimer(DestroyCountdownTimer, this, &ACube::OnDestructionTimerEnd, LifespanSecondsAfterCut, false);
}

FVector ACube::GetMovementDirection()
{
	return WorldMovementDirection;
}


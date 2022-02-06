// Fill out your copyright notice in the Description page of Project Settings.


#include "Saber.h"

#include "Cuttable.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASaber::ASaber(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));
	SaberMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SaberMesh"));
	HandleMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("HandleMesh"));
	SaberCollision = OI.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("SaberCollision"));
	SaberFX = OI.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("SaberFX"));

	SaberMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	HandleMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	SaberCollision->AttachToComponent(SaberMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SaberFX->AttachToComponent(SaberMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SetRootComponent(SceneRoot);

	SaberMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ASaber::BeginPlay()
{
	Super::BeginPlay();
	
	SetSaberRetracted(true);
	GetWorldTimerManager().SetTimer(DemoSaberActivation, this, &ASaber::DemoToggleSaber, 3.f, true);

	if (SaberCollision != nullptr)
	{
		SaberCollision->OnComponentBeginOverlap.AddDynamic(this, &ASaber::OnOverlapBegin);
	}
}

void ASaber::DemoToggleSaber()
{
	//SetSaberRetracted(!bIsRetracted);
}

// Called every frame
void ASaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRetractionState(DeltaTime);

	SaberTipLastFrame = SaberTipCurrentFrame;
	SaberTipCurrentFrame = GetActorUpVector();
}

void ASaber::UpdateRetractionState(float DeltaTime)
{
	CurrentSaberRetraction = FMath::FInterpConstantTo(CurrentSaberRetraction, TargetSaberRetraction, DeltaTime, SaberRetractionSpeed);
	if (SaberDynamicMaterial != nullptr)
	{
		SaberDynamicMaterial->SetScalarParameterValue(TEXT("Retraction"), CurrentSaberRetraction);
	}
}

void ASaber::SetSaberRetracted(bool IsRetracted)
{
	TargetSaberRetraction = IsRetracted ? SaberRetractionMax : SaberRetractionMin;
	if (SaberFX != nullptr)
	{
		SaberFX->SetBoolParameter(TEXT("IsEmitterActive"), IsRetracted);
	}
	bIsRetracted = IsRetracted;
}

void ASaber::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	bool IsRed = SaberColor == Red;

	if (SaberMesh != nullptr)
	{
		if (SaberDynamicMaterial == nullptr)
		{
			SaberDynamicMaterial = UMaterialInstanceDynamic::Create(SaberMesh->GetMaterial(0), nullptr);
			SaberMesh->SetMaterial(0, SaberDynamicMaterial);
		}

		UStaticMesh* Mesh = SaberMesh->GetStaticMesh();
		if (Mesh != nullptr && SaberCollision != nullptr)
		{
			FVector MeshExtent = Mesh->GetBoundingBox().GetSize();
			float HalfHeight = MeshExtent.Z / 2.f;
			float HalfWidth = FMath::Max(MeshExtent.X, MeshExtent.Y) / 2.f;
			SaberCollision->SetCapsuleSize(HalfWidth, HalfHeight);
			SaberCollision->SetRelativeLocation(FVector(0.f, 0.f, HalfHeight));
		}
	}
	SaberDynamicMaterial->SetScalarParameterValue(TEXT("IsRed"), IsRed);

	if (SaberFX != nullptr)
	{
		SaberFX->SetBoolParameter(TEXT("IsRed"), IsRed);
	}
}

void ASaber::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsRetracted)
	{
		return; // Inactive saber can't cut
	}
	ICuttable* CuttableActor = Cast<ICuttable>(OtherActor);
	if (CuttableActor != nullptr)
	{
		// Single point on cutter plane
		FVector BasePoint = GetActorLocation();
		// Sum of vector speeds in the moment of saber hitting the object
		FVector CollisionSummary = SaberTipCurrentFrame - SaberTipLastFrame - CuttableActor->GetMovementDirection();
		// Derive cutter plane normal
		FVector CutterPlaneNormal = FVector::CrossProduct(CollisionSummary, SaberTipLastFrame);
		CuttableActor->CutOccured(BasePoint, CutterPlaneNormal);
	}
}


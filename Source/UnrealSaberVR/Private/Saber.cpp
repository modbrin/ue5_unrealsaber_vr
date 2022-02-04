// Fill out your copyright notice in the Description page of Project Settings.


#include "Saber.h"
#include "NiagaraComponent.h"

// Sets default values
ASaber::ASaber(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));
	SaberMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SaberMesh"));
	HandleMesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("HandleMesh"));
	SaberFX = OI.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("SaberFX"));

	SaberMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	HandleMesh->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	SaberFX->AttachToComponent(SaberMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SetRootComponent(SceneRoot);
}

// Called when the game starts or when spawned
void ASaber::BeginPlay()
{
	Super::BeginPlay();
	
	SetSaberRetracted(false);
	GetWorldTimerManager().SetTimer(DemoSaberActivation, this, &ASaber::DemoToggleSaber, 3.f, true);
}

void ASaber::DemoToggleSaber()
{
	SetSaberRetracted(!bIsRetracted);
}

// Called every frame
void ASaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRetractionState(DeltaTime);	
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

	bool IsRed = SaberColor == Red ? true : false;

	if (SaberDynamicMaterial == nullptr)
	{
		SaberDynamicMaterial = UMaterialInstanceDynamic::Create(SaberMesh->GetMaterial(0), nullptr);
	}
	if (SaberMesh != nullptr)
	{
		SaberMesh->SetMaterial(0, SaberDynamicMaterial);
	}
	SaberDynamicMaterial->SetScalarParameterValue(TEXT("IsRed"), IsRed);

	if (SaberFX != nullptr)
	{
		SaberFX->SetBoolParameter(TEXT("IsRed"), IsRed);
	}
}


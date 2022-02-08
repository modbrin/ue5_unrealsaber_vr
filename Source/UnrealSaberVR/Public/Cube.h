// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cuttable.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class UNREALSABERVR_API ACube : public AActor, public ICuttable
{
	GENERATED_BODY()
	
public:	
	ACube(const FObjectInitializer& OI);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void PerformMovement(float DeltaTime);
	void Hit(FVector HitLocation, FVector HitNormal, FVector SaberDirection);
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnDestructionTimerEnd();

	// ICuttable Implementation
	virtual void CutOccured(FVector CutterPlanePoint, FVector CutterPlaneNormal) override;
	virtual FVector GetMovementDirection() override;
	// ICuttable Implementation

public: // properties
	UPROPERTY()
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* CubeCounterpartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	FVector WorldMovementDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SideImpulseOnCut = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LifespanSecondsAfterCut = 3.f;
	
private: // properties
	bool bIsFlying = true;

	UPROPERTY()
	UMaterialInstanceDynamic* CubeDynamicMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* CubeCounterpartDynamicMaterial;

	FTimerHandle DestroyCountdownTimer;
};

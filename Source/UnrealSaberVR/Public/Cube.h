// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class UNREALSABERVR_API ACube : public AActor
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

public: // properties
	UPROPERTY()
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* CubeMeshCounterpart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	FVector WorldMovementDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 5.f;

private: // properties
	bool bIsFlying = true;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Saber.generated.h"

UENUM(BlueprintType)
enum ESaberColor
{
	Blue = 0 UMETA("Blue"),
	Red = 1 UMETA("Red"),
};

UCLASS()
class UNREALSABERVR_API ASaber : public AActor
{
	GENERATED_BODY()
	
public:	
	ASaber(const FObjectInitializer& OI);

protected:
	virtual void BeginPlay() override;
	void DemoToggleSaber();

public:	
	virtual void Tick(float DeltaTime) override;
	void UpdateRetractionState(float DeltaTime);
	void SetSaberRetracted(bool IsRetracted);
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public: // properties
	UPROPERTY()
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SaberMesh; 

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* HandleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* SaberCollision;
	
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* SaberFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TargetSaberRetraction = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentSaberRetraction = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float SaberRetractionMin = -100.f;

	UPROPERTY(EditDefaultsOnly)
	float SaberRetractionMax = 3000.f;

	UPROPERTY(EditDefaultsOnly)
	float SaberRetractionSpeed = 7000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESaberColor> SaberColor = ESaberColor::Blue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRetracted = false;
private: // properties
	UPROPERTY()
	UMaterialInstanceDynamic* SaberDynamicMaterial;

	FTimerHandle DemoSaberActivation;

	FVector SaberTipLastFrame = FVector(0.f, 0.f, 1.f);
	FVector SaberTipCurrentFrame = FVector(0.f, 0.f, 1.f);
};

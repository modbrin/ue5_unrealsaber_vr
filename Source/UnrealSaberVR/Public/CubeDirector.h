// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cube.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "CubeDirector.generated.h"

UCLASS()
class UNREALSABERVR_API ACubeDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeDirector(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TimerTick();
public: // properties
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACube> CubeClass;

	UPROPERTY()
	UArrowComponent* ArrowComponent;
	
	
private: // properties
	FTimerHandle SpawnTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDirector.h"

// Sets default values
ACubeDirector::ACubeDirector(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = OI.CreateDefaultSubobject<UArrowComponent>(this, TEXT("ActorRootArrow"));
	SetRootComponent(ArrowComponent);
}

// Called when the game starts or when spawned
void ACubeDirector::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACubeDirector::TimerTick, 1.f, true);
}

// Called every frame
void ACubeDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDirector::TimerTick()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		FVector Location = GetActorLocation();
		World->SpawnActor(CubeClass, &Location);
	}
}


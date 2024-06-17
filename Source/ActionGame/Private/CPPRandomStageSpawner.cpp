// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPRandomStageSpawner.h"
#include "CPPStageFloor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPPRandomStageSpawner::ACPPRandomStageSpawner()
{

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("SceneRoot")));
	RootComponent = SceneRoot;

	StageWall = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StageWall")));
	StageWall->SetupAttachment(RootComponent);

	Area = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Area")));
	Area->SetupAttachment(StageWall);

}

void ACPPRandomStageSpawner::SpawnFloor()
{
	if (!Floor) return;

	FVector StageCenter = Area->GetComponentTransform().GetLocation();
	FVector StageExtent = Area->GetScaledBoxExtent();

	for (uint8 i = 0; i < NumberOfFloor; i++) {
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(
			UKismetMathLibrary::RandomPointInBoundingBox(StageCenter, StageExtent),
			FRotator::ZeroRotator,
			FVector(1.f, 1.f, 1.f)
		);
		GetWorld()->SpawnActor<ACPPStageFloor>(Floor, SpawnTransform, FActorSpawnParameters());
	}
}

// Called when the game starts or when spawned
void ACPPRandomStageSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnFloor();
	
}

// Called every frame
void ACPPRandomStageSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPStageFloor.h"

// Sets default values
ACPPStageFloor::ACPPStageFloor()
{
	UE_LOG(LogTemp, Log, TEXT("MyLog: StageFloorConstructorBegin"));

	PrimaryActorTick.bCanEverTick = true;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Floor"));
	Floor->SetupAttachment(RootComponent);
	UE_LOG(LogTemp, Log, TEXT("MyLog: StageFloorConstructorEnd"));

}

// Called when the game starts or when spawned
void ACPPStageFloor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPPStageFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


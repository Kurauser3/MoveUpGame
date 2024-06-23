// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPStageFloor.h"

// Sets default values
ACPPStageFloor::ACPPStageFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Floor"));
	Floor->SetupAttachment(RootComponent);

	
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


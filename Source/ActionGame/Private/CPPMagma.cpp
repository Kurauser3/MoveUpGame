// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMagma.h"

// Sets default values
ACPPMagma::ACPPMagma()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("SceneRoot")));
	RootComponent = SceneRoot;

	MagmaMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Magma")));
	MagmaMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPPMagma::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPPMagma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveUp(DeltaTime);
}

void ACPPMagma::MoveUp(float DeltaTime)
{
	FVector Location = GetActorLocation();
	Location.Z += DeltaTime * Speed;
	SetActorLocation(Location);
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("%f"), Location.Z));
}
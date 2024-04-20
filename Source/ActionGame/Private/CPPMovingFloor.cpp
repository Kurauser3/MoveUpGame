// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMovingFloor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CPPMovingPointToPointComponent.h"

// Sets default values
ACPPMovingFloor::ACPPMovingFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup component tree
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Developers/tkr31/Meshes/SM_Floor400"));
	Plane->SetStaticMesh(Mesh);
	Plane->SetupAttachment(RootComponent);

	Moving = CreateDefaultSubobject<UCPPMovingPointToPointComponent>(TEXT("Moving"));


}

// Called when the game starts or when spawned
void ACPPMovingFloor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPPMovingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


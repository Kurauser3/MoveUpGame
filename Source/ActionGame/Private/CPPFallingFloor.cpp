// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPFallingFloor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VanishingFloorComponent.h"

// Sets default values
ACPPFallingFloor::ACPPFallingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup component tree
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Developers/tkr31/Meshes/SM_Floor400"));
	Floor->SetStaticMesh(Mesh);
	Floor->SetupAttachment(RootComponent);

	VanishingFloor = CreateDefaultSubobject<UVanishingFloorComponent>(TEXT("VanishingFloorComponent"));

}

// Called when the game starts or when spawned
void ACPPFallingFloor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACPPFallingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPReflectionMovingFloor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACPPReflectionMovingFloor::ACPPReflectionMovingFloor()
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

}

// Called when the game starts or when spawned
void ACPPReflectionMovingFloor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	Location = InitialLocation;

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f %f %f"), InitialLocation.X, InitialLocation.Y, InitialLocation.Z));

}

// Called every frame
void ACPPReflectionMovingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Movement = FVector(
		DeltaTime * Velocity.X * (bIsMovingLeft ? -1.0 : 1.0),
		DeltaTime * Velocity.Y * (bIsMovingBack ? -1.0 : 1.0),
		DeltaTime * Velocity.Z * (bIsMovingDown ? -1.0 : 1.0)
	);

	FVector MovedLocation = FVector(
		Location.X + Movement.X,
		Location.Y + Movement.Y,
		Location.Z + Movement.Z
	);

	auto GetExceededMovement = [this, MovedLocation]() -> FVector
		{
			return FVector(
				abs(MovedLocation.X - InitialLocation.X) - MovableRange.X / 2.0,
				abs(MovedLocation.Y - InitialLocation.Y) - MovableRange.Y / 2.0,
				abs(MovedLocation.Z - InitialLocation.Z) - MovableRange.Z / 2.0
			);
		};

	// The excess is the amount of movement in the opposite direction.
	if (GetExceededMovement().X > 0.0) {
		bIsMovingLeft = !bIsMovingLeft;
		MovedLocation.X += GetExceededMovement().X * 2.0 * (bIsMovingLeft ? -1.0 : 1.0);
	}
	if (GetExceededMovement().Y > 0.0) {
		bIsMovingBack = !bIsMovingBack;
		MovedLocation.Y += GetExceededMovement().Y * 2.0 * (bIsMovingBack ? -1.0 : 1.0);
	}
	if (GetExceededMovement().Z > 0.0) {
		bIsMovingDown = !bIsMovingDown;
		MovedLocation.Z += GetExceededMovement().Z * 2.0 * (bIsMovingDown ? -1.0 : 1.0);
	}

	Location = MovedLocation;
	SetActorLocation(Location);
}

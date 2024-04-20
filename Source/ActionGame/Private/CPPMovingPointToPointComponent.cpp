// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMovingPointToPointComponent.h"

// Sets default values for this component's properties
UCPPMovingPointToPointComponent::UCPPMovingPointToPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCPPMovingPointToPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initial location
	Location = GetOwner()->GetActorLocation();
	// Setup Checkpoints.
	AllCheckpoints.Emplace(Location);
	AllCheckpoints.Append(Checkpoints);
	PastCheckpoint = 0;
}


// Called every frame
void UCPPMovingPointToPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int32 NextCheckpoint = PastCheckpoint + 1;
	if (NextCheckpoint > AllCheckpoints.Num() - 1)
	{
		NextCheckpoint = 0;
	}

	FVector VectorDiff = AllCheckpoints[NextCheckpoint] - Location;
	FVector VectorDiffNorm = VectorDiff.GetSafeNormal();

	Velocity.Set(
		CompositeVelocity * VectorDiffNorm.X,
		CompositeVelocity * VectorDiffNorm.Y,
		CompositeVelocity * VectorDiffNorm.Z
	);

	FVector Movement = FVector(
		DeltaTime * Velocity.X,
		DeltaTime * Velocity.Y,
		DeltaTime * Velocity.Z
	);

	// Determine if the amount of movement exceeds the checkpoint.
	auto bIsExceededMovementX = [Movement, VectorDiff]() -> bool
		{
			return abs(VectorDiff.X) < abs(Movement.X);
		};
	auto bIsExceededMovementY = [Movement, VectorDiff]() -> bool
		{
			return abs(VectorDiff.Y) < abs(Movement.Y);
		};
	auto bIsExceededMovementZ = [Movement, VectorDiff]() -> bool
		{
			return abs(VectorDiff.Z) < abs(Movement.Z);
		};

	FVector MovedLocation = FVector(
		Location.X + Movement.X,
		Location.Y + Movement.Y,
		Location.Z + Movement.Z
	);

	if (bIsExceededMovementX()) {
		MovedLocation.X = AllCheckpoints[NextCheckpoint].X;
	}
	if (bIsExceededMovementY()) {
		MovedLocation.Y = AllCheckpoints[NextCheckpoint].Y;
	}
	if (bIsExceededMovementZ()) {
		MovedLocation.Z = AllCheckpoints[NextCheckpoint].Z;
	}

	if (MovedLocation == AllCheckpoints[NextCheckpoint]) {
		PastCheckpoint = NextCheckpoint;
	}

	Location = MovedLocation;
	GetOwner()->SetActorLocation(Location);
}


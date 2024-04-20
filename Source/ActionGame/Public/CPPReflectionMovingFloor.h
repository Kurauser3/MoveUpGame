// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPReflectionMovingFloor.generated.h"

UCLASS()
class ACTIONGAME_API ACPPReflectionMovingFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPPReflectionMovingFloor();

	// Component tree
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(EditAnywhere);
	TObjectPtr<UStaticMeshComponent> Plane;

	/** The actual amount of movement is halved on each side. */
	UPROPERTY(EditAnywhere)
	FVector MovableRange = FVector(300.0, 300.0, 100.0);

	UPROPERTY(EditAnywhere)
	FVector Velocity = FVector(200.0, 200.0, 0.0);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector InitialLocation;
	FVector Location;

	bool bIsMovingLeft = true;
	bool bIsMovingDown = true;
	bool bIsMovingBack = true;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
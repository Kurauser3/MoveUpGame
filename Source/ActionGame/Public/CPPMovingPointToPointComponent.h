// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPPMovingPointToPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UCPPMovingPointToPointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPPMovingPointToPointComponent();

	/** Excluding initial location */
	UPROPERTY(EditAnywhere)
	TArray<FVector> Checkpoints = { FVector(200.0, 400.0, 700.0) };

	UPROPERTY(EditAnywhere)
	double CompositeVelocity = 100.0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector Location;
	FVector Velocity = FVector(0.0, 0.0, 0.0);
	/** Including initial location */
	TArray<FVector> AllCheckpoints;
	int32 PastCheckpoint;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

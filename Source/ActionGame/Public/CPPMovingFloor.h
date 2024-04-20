// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPMovingFloor.generated.h"

UCLASS()
class ACTIONGAME_API ACPPMovingFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPMovingFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Component tree
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(EditAnywhere);
	TObjectPtr<UStaticMeshComponent> Plane;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCPPMovingPointToPointComponent> Moving;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
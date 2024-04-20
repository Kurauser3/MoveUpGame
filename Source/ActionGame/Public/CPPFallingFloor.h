// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPFallingFloor.generated.h"

UCLASS()
class ACTIONGAME_API ACPPFallingFloor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACPPFallingFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Component tree
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UVanishingFloorComponent> VanishingFloor;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(EditAnywhere);
	TObjectPtr<UStaticMeshComponent> Floor;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

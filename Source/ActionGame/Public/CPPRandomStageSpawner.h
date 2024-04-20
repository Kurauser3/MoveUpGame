// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPRandomStageSpawner.generated.h"

UCLASS()
class ACTIONGAME_API ACPPRandomStageSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPRandomStageSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> StageWall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBoxComponent> Area;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StageFloor")
	TSubclassOf<class ACPPStageFloor> Floor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageFloor")
	uint8 NumberOfFloor = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintAuthorityOnly)
	void SpawnFloor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

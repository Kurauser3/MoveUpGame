// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPStageFloor.generated.h"

UCLASS()
class ACTIONGAME_API ACPPStageFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPStageFloor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> Floor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBoxComponent> NextStepArea;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

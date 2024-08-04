// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPMagma.generated.h"

UCLASS()
class ACTIONGAME_API ACPPMagma : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
	void MoveUp(float DeltaTime);

public:	
	// Sets default values for this actor's properties
	ACPPMagma();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double BaseSpeed = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double Speed = 0.f;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MagmaMesh;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// TObjectPtr<class UProjectileMovementComponent> Movement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPMagma.h"

#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ACPPMagma::ACPPMagma()
{
	UE_LOG(LogTemp, Log, TEXT("MyLog: MagmaConstructorBegin"));

	PrimaryActorTick.bCanEverTick = true;

	/*
	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("SceneRoot")));
	RootComponent = SceneRoot;
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(FName(TEXT("Movement")));
	Movement->UpdatedComponent = MagmaMesh;
	Movement->InitialSpeed = BaseSpeed;
	Movement->MaxSpeed = 300;
	Movement->Velocity = FVector(0.0, 0.0, 1.0);
	Movement->ProjectileGravityScale = 0;
	*/

	MagmaMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Magma")));
	RootComponent = MagmaMesh;
	// MagmaMesh->SetupAttachment(SceneRoot);

	MagmaMesh->SetNotifyRigidBodyCollision(true); // Hit�C�x���g�𐶐�����
	UE_LOG(LogTemp, Log, TEXT("MyLog: MagmaConstructorEnd"));

}

// Called when the game starts or when spawned
void ACPPMagma::BeginPlay()
{
	Super::BeginPlay();

	Speed = BaseSpeed;
}

// Called every frame
void ACPPMagma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveUp(DeltaTime);
}

void ACPPMagma::MoveUp(float DeltaTime)
{
	FVector Location = GetActorLocation();
	Location.Z += DeltaTime * Speed;
	static FHitResult HitResult;
	SetActorLocation(Location, true, &HitResult);
	if (HitResult.bBlockingHit || HitResult.bStartPenetrating)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, TEXT("HitHitHit!!!"));

	}
}
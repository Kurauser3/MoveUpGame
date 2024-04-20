// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CPPPlayer.generated.h"

UCLASS()
class ACTIONGAME_API ACPPPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPPlayer();

	// カメラ ------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	// アクション --------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveCameraAction;

	// アニメーション ---------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class ULevelSequence* WalkSequence;

protected:

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void Walk(const FInputActionValue& Value);

	void MoveCamera(const FInputActionValue& Value);

	FVector TestCameraVectorF = FVector(0.0, 0.0, 0.0);
	FVector TestCameraVectorR = FVector(0.0, 0.0, 0.0);
	void TestJump();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

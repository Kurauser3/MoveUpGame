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

	UPROPERTY(VisibleAnywhere)
	float JumpVelocity = 250.f;

	UPROPERTY(VisibleAnywhere)
	bool bJumpChargeStarted = false;

public:
	// Sets default values for this character's properties
	ACPPPlayer();

	// カメラ ------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	// アクション --------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpChargeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveCameraAction;

	// アニメーション ---------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class ULevelSequence* WalkSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limitation")
	float MinJumpVelocity = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limitation")
	float MaxJumpVelocity = 500.f;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Sprint(const FInputActionValue& Value);

	UFUNCTION()
	void Walk(const FInputActionValue& Value);

	UFUNCTION()
	void MoveCamera(const FInputActionValue& Value);

	FVector TestCameraVectorF = FVector(0.0, 0.0, 0.0);
	FVector TestCameraVectorR = FVector(0.0, 0.0, 0.0);
	UFUNCTION()
	void TestJump();

	UFUNCTION()
	void JumpCharge(const FInputActionInstance& Value);

	UFUNCTION()
	void JumpChargeStarted();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

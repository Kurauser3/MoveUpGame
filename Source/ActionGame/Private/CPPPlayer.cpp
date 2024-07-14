// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ACPPPlayer::ACPPPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);	// �����蔻��͈�

	// �ړ��̃p�����[�^�[
	bUseControllerRotationYaw = false;	// �L�����N�^�[�̌������R���g���[���[�̌����ɒǏ]�����Ȃ�
	GetCharacterMovement()->bOrientRotationToMovement = true;	// �L�����N�^�[�̐��ʂ��ړ������Ɍ�����
	GetCharacterMovement()->AirControl = 0.75f;
	GetCharacterMovement()->GravityScale = 0.4f;
	GetCharacterMovement()->MaxStepHeight = 5.0f;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->MaxAcceleration = 400.0f;	// �����A����̉���
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;	// �ړ����͂��Ȃ��Ȃ����Ƃ��̋t����

	// �X�v�����O�A�[��
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f;
	// �J����
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);


}

void ACPPPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	CameraBoom->bUsePawnControlRotation = true;	// �J�����𓮂����̂ɕK�v�Ȑݒ�(�R���g���[���[�̌����ɒǏ]����)
	
}

void ACPPPlayer::KillOwn()
{
	GetController()->UnPossess();
	SetActorEnableCollision(false);
}


// �v���C���[�̑��� -----------------------------------------------------------------------------------------------------------------------------

void ACPPPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// �e�A�N�V�������o�C���h
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPPPlayer::TestJump);
		EnhancedInputComponent->BindAction(JumpChargeAction, ETriggerEvent::Started, this, &ACPPPlayer::JumpChargeStarted);
		EnhancedInputComponent->BindAction(JumpChargeAction, ETriggerEvent::Ongoing, this, &ACPPPlayer::JumpCharge);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPPlayer::Move);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACPPPlayer::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACPPPlayer::Walk);
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &ACPPPlayer::MoveCamera);

	}
}

void ACPPPlayer::Move(const FInputActionValue& Value)
{
	if (bJumpChargeStarted) return;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	FVector2D Input = Value.Get<FVector2D>();

	AddMovementInput(Camera->GetForwardVector().GetSafeNormal2D(), Input.Y);
	AddMovementInput(Camera->GetRightVector().GetSafeNormal2D(), Input.X);

}

void ACPPPlayer::Sprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ACPPPlayer::Walk(const FInputActionValue& Value)
{
	if (bJumpChargeStarted) return;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ACPPPlayer::MoveCamera(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	TestCameraVectorF = Camera->GetForwardVector();
	TestCameraVectorR = Camera->GetRightVector();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);

}

void ACPPPlayer::TestJump()
{
	if (!bJumpChargeStarted) return;
	bJumpChargeStarted = false;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	Jump();
	JumpVelocity = MinJumpVelocity;
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f, %f, %f"), TestCameraVectorF.X, TestCameraVectorF.Y, TestCameraVectorF.Z));
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f, %f, %f"), TestCameraVectorR.X, TestCameraVectorR.Y, TestCameraVectorR.Z));
}

void ACPPPlayer::JumpCharge(const FInputActionInstance& Value)
{
	if (!CanJump()) bJumpChargeStarted = false;
	if (!bJumpChargeStarted) return;
	JumpVelocity += Value.GetElapsedTime() * JumpChargeSpeed;
	if (JumpVelocity > MaxJumpVelocity)	JumpVelocity = MaxJumpVelocity;
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Charge: %f"), JumpVelocity));
}

void ACPPPlayer::JumpChargeStarted()
{
	bJumpChargeStarted = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
}

void ACPPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

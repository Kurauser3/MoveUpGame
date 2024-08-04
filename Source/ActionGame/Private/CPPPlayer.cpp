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
	UE_LOG(LogTemp, Log, TEXT("MyLog: PlayerConstructorBegin"));

	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);	// 当たり判定範囲

	// 移動のパラメーター
	bUseControllerRotationYaw = false;	// キャラクターの向きをコントローラーの向きに追従させない
	GetCharacterMovement()->bOrientRotationToMovement = true;	// キャラクターの正面を移動方向に向ける
	GetCharacterMovement()->AirControl = 0.75f;
	GetCharacterMovement()->GravityScale = 0.4f;
	GetCharacterMovement()->MaxStepHeight = 5.0f;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->MaxAcceleration = 400.0f;	// 歩き、走りの加速
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;	// 移動入力がなくなったときの逆加速

	// スプリングアーム
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f;
	// カメラ
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	UE_LOG(LogTemp, Log, TEXT("MyLog: PlayerConstructorEnd"));

}

void ACPPPlayer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCustomMode);

	// 着地でタイマの値を取り、落ち始めからの経過時間をイベント関数に投げる
	GetTimeOnFallingFinished(PrevMovementMode, PrevCustomMode);
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
	CameraBoom->bUsePawnControlRotation = true;	// カメラを動かすのに必要な設定(コントローラーの向きに追従する)

}


void ACPPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFallingAfterJump();
}


void ACPPPlayer::KillOwn()
{
	GetController()->UnPossess();

	// 物理に任せる
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

}

void ACPPPlayer::CheckFallingAfterJump()
{
	if (!bJumpStarted) return;
	if (GetCharacterMovement()->Velocity.Z >= 0) return;
	if (bIsFallingAfterJump) return;
	bIsFallingAfterJump = true;
	OnStartFallingAfterJump();
}

void ACPPPlayer::OnStartFallingAfterJump()
{
	GetTimeOnFallingStarted();
}

void ACPPPlayer::GetTimeOnFallingStarted()
{

	GetWorldTimerManager().SetTimer(Timer, 60.f, true);
	FallingStarted = GetWorldTimerManager().GetTimerElapsed(Timer);

}

void ACPPPlayer::GetTimeOnFallingFinished(EMovementMode PrevMovementMode, uint8 PrevCustomMode)
{
	if (PrevMovementMode != EMovementMode::MOVE_Falling) return;
	if (!bJumpStarted) return;
	bJumpStarted = false;

	if (!bIsFallingAfterJump) {
		OnGetFallingTime.Broadcast(this, 0.f);
		return;
	}
	bIsFallingAfterJump = false;
		;
	FallingFinished = GetWorldTimerManager().GetTimerElapsed(Timer);
	GetWorldTimerManager().ClearTimer(Timer);

	if (FallingFinished - FallingStarted < 0.f) return;
	OnGetFallingTime.Broadcast(this, FallingFinished - FallingStarted);

}

// プレイヤーの操作 -----------------------------------------------------------------------------------------------------------------------------

void ACPPPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// 各アクションをバインド
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

// デバッグ用のキーバインドを用意するのが面倒なのでジャンプをトリガーに色々出力してみる
void ACPPPlayer::TestJump()
{
	// ジャンプの処理
	if (!bJumpChargeStarted) return;
	bJumpChargeStarted = false;
	LocationJumpingStarted = GetActorLocation();
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	Jump();
	bJumpStarted = true;
	OnJump.Broadcast(this);
	JumpVelocity = MinJumpVelocity;
	
	// ここからテスト、デバッグ用の内容
	// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f, %f, %f"), TestCameraVectorF.X, TestCameraVectorF.Y, TestCameraVectorF.Z));
	// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f, %f, %f"), TestCameraVectorR.X, TestCameraVectorR.Y, TestCameraVectorR.Z));
}

void ACPPPlayer::JumpCharge(const FInputActionInstance& Value)
{
	if (!bJumpChargeStarted) return;
	JumpVelocity += Value.GetElapsedTime() * JumpChargeSpeed;
	if (JumpVelocity > MaxJumpVelocity)	JumpVelocity = MaxJumpVelocity;
	OnCharge.Broadcast(this, JumpVelocity, MinJumpVelocity, MaxJumpVelocity);
}

void ACPPPlayer::JumpChargeStarted()
{
	bJumpChargeStarted = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	if (!CanJump()) bJumpChargeStarted = false;
}

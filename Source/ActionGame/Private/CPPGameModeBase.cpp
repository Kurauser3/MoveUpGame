// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPGameModeBase.h"
#include "CPPRandomStageSpawner.h"
#include "CPPMagma.h"
#include "CPPPlayer.h"
#include "CPPPlayerState.h"
#include "CPPGameObserver.h"
#include "CPPGameHUD.h"

#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/TriggerVolume.h>
#include <Engine/TargetPoint.h>


ACPPGameModeBase::ACPPGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Developers/tkr31/Characters/BP_Player"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }

}

void ACPPGameModeBase::BeginPlay()
{

    // ����UI����J�ڂ��Ă����ꍇ�̓}�E�X������Q�[���ɖ߂��K�v������̂ł����Őݒ�
    TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(this, 0);
    if (Controller)
    {
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
        Controller->SetShowMouseCursor(false);
    }

    TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPRandomStageSpawner::StaticClass());
    PrevStage = Cast<ACPPRandomStageSpawner>(ActorToFind);
    if (!PrevStage) return;

    // �ŏ��̃X�e�[�W�X�|�i�[��ݒ肷��
    TArray<TObjectPtr<AActor>> ActorsToFind;
    UGameplayStatics::GetAllActorsOfClass(this, ATriggerVolume::StaticClass(), ActorsToFind);
    for (TObjectPtr<AActor> TriggerVolume : ActorsToFind)
    {
        TObjectPtr<ATriggerVolume> TriggerVolumeCast = Cast<ATriggerVolume>(TriggerVolume);
        if (TriggerVolumeCast)
            TriggerVolumeCast->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::HandleOverlapSpawnTrigger);
    }

    // �v���C���[���W�����v�̗��߂����Ă���ԃ`���[�W�i�����v���O���X�o�[�ŕ\������
    ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPPlayer::StaticClass());
    TObjectPtr<ACPPPlayer> Player = Cast<ACPPPlayer>(ActorToFind);
    if (Player)
    {
        Player->OnCharge.AddDynamic(this, &ACPPGameModeBase::HandleCharacterCharging);
        Player->OnJump.AddDynamic(this, &ACPPGameModeBase::HandleCharacterJump);
        Player->OnGetFallingTime.AddDynamic(this, &ACPPGameModeBase::HandleCharacterLanding);
    }

    // �}�O�}�ɓ���������Q�[���I�[�o�[ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    /*
    �{���̓L�����N�^�[�Ƀq�b�g�A���邢�̓I�[�o�[���b�v�������ɍ쓮���Ăق������A�L�����N�^�[�������Ă��Ȃ����Ɍ����Ȃ������̂ŕۗ�
    ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPMagma::StaticClass());
    TObjectPtr<ACPPMagma> Magma = Cast<ACPPMagma>(ActorToFind);
    if (Magma)
        Magma->OnActorHit.AddDynamic(this, &ACPPGameModeBase::HandleOverlapMagma);
    */
    // ����ɃI�u�U�[�o�[�N���X������Ă�������ʒm���Ă��炤(��Ńq�b�g������������Ē���)
    ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPGameObserver::StaticClass());
    TObjectPtr<ACPPGameObserver> Observer = Cast<ACPPGameObserver>(ActorToFind);
    if (Observer)
        Observer->OnMagmaHitCharacter.AddDynamic(this, &ACPPGameModeBase::HandleOverlapMagmaTEMP);

}

void ACPPGameModeBase::HandleOverlapSpawnTrigger(AActor* OverlappedActor, AActor* OtherActor)
{
    SpawnNext(OverlappedActor, OtherActor);
    IncreaseMagmaSpeed(1);
}

// ���q�b�g���肪���܂������Ă��Ȃ����߁A�d�l��ۗ����Ă��鏈��
void ACPPGameModeBase::HandleOverlapMagma(AActor* OverlappedActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    ACPPPlayer* Player = Cast<ACPPPlayer>(OverlappedActor);
    if (!Player) return;
    ACPPMagma* Magma = Cast<ACPPMagma>(OtherActor);
    if (!Magma) return;
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    if (PlayerController)
    {
        ACPPGameHUD* GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
        if (GameHUD) GameHUD->ShowGameOver();
    }
    Player->KillOwn(); // �v���C���[���L��(�����؂藣��)
}

void ACPPGameModeBase::HandleOverlapMagmaTEMP(class ACPPMagma* Magma, class ACPPPlayer* Player)
{
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    if (PlayerController)
    {
        ACPPGameHUD* GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
        if (GameHUD) GameHUD->ShowGameOver();
    }
    Player->KillOwn(); // �v���C���[���L��(�����؂藣��)

}

void ACPPGameModeBase::HandleCharacterCharging(ACPPPlayer* Player, float JumpVelocity, float Min, float Max)
{
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    ACPPGameHUD* GameHUD = nullptr;
    if (PlayerController) GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
    if (!GameHUD) return;
    if (!bChargingProgressShown)
    {
        GameHUD->ShowCharacterState();
        bChargingProgressShown = true;
    }
    if (GameHUD->PlayerStateWidget) GameHUD->PlayerStateWidget->SetChargingProgress(JumpVelocity, Max, Min);
}

void ACPPGameModeBase::HandleCharacterJump(ACPPPlayer* Player)
{
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    ACPPGameHUD* GameHUD = nullptr;
    if (PlayerController) GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
    if (GameHUD) GameHUD->RemoveCharacterState();
    bChargingProgressShown = false;
}

void ACPPGameModeBase::HandleCharacterLanding(ACPPPlayer* Player, float FallingTime)
{
    if (Player->LocationJumpingStarted.Z + 3.0 >= Player->GetActorLocation().Z) return;

    EJumpingEvaluation Eval = EvaluateJumping(FallingTime);

    // �]������ʂɏo��
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    ACPPGameHUD* GameHUD = nullptr;
    if (PlayerController) GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
    if (GameHUD)GameHUD->SetEvaluation(Eval);

    IncreaseJumpScore(Player, JumpScoreMap[Eval]);
}

void ACPPGameModeBase::SpawnNext(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!Stage) return; // ��������X�e�[�W���ݒ肳��Ă��Ȃ���Ζ���

    ACharacter* Player = Cast<ACharacter>(OtherActor);
    if (!Player) return; // �I�[�o�[���b�v�����̂��L�����N�^�[�łȂ���Ζ���

    TObjectPtr<ATriggerVolume> TriggerVolume = Cast<ATriggerVolume>(OverlappedActor);
    if (!TriggerVolume) return; // ���Ό����g���K�[�{�����[���łȂ���Ζ���

    TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ATargetPoint::StaticClass());
    TObjectPtr<ATargetPoint> TargetPoint = Cast<ATargetPoint>(ActorToFind);
    if (!TargetPoint) return; // ���̐����|�C���g��������Ȃ���Ζ���

    // ���̃g���K�[�{�����[���𐶐�����
    ATriggerVolume* NewTriggerVolume = SpawnNextTrigger(GetWorld(), TriggerVolume, TargetPoint->GetActorLocation());
    // ���̃g���K�[�ł����l�̏���������悤�ɂ���
    NewTriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::HandleOverlapSpawnTrigger);

    // �X�e�[�W����
    PrevStage = SpawnNextStage(GetWorld(), Stage, TargetPoint->GetActorLocation(), PrevStage->NextFloorLocation);

    // ���̃X�|�[���n�_�𐶐�����
    FVector NextLocation = TargetPoint->GetActorLocation();
    NextLocation.Z += PrevStage->WallMaxVextor.Z - PrevStage->WallMinVector.Z;
    SpawnNextStageMakingPoint(
        GetWorld(),
        NextLocation
    );
    // �s�v�ɂȂ����A�N�^�͏����Ă���
    TriggerVolume->Destroy();
    TargetPoint->Destroy();

    IncreaseProgress(Player);
}

void ACPPGameModeBase::IncreaseMagmaSpeed(uint16 increment)
{
    TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPMagma::StaticClass());
    TObjectPtr<ACPPMagma> Magma = Cast<ACPPMagma>(ActorToFind);
    if (!Magma) return;

    Magma->Speed = Magma->Speed + increment;
    if (Magma->Speed <= MagmaMaxSpeed) return;
    Magma->Speed = MagmaMaxSpeed;
}

// static�Ȃ�� --------------------------------------------------------------------------------------------------------------------------------------------

void ACPPGameModeBase::IncreaseProgress(ACharacter* Player)
{
    if (!Player) return;
 
    ACPPPlayer* ExclusivePlayer = Cast<ACPPPlayer>(Player);
    if (!ExclusivePlayer) return;

    ACPPPlayerState* State = Cast<ACPPPlayerState>(ExclusivePlayer->GetPlayerState());
    if (!State) return;
    State->SetProgress(State->GetProgress() + 1);
 
    // �X�R�A�\��
    ShowScore(ExclusivePlayer);
}

EJumpingEvaluation ACPPGameModeBase::EvaluateJumping(float FallingTime)
{
    if (FallingTime < 0.3f) return EJumpingEvaluation::EVAL_Perfect;
    if (FallingTime < 0.5f) return EJumpingEvaluation::EVAL_Good;
    return EJumpingEvaluation::EVAL_Bad;
}

void ACPPGameModeBase::IncreaseJumpScore(ACharacter* Player, float Score)
{
    if (!Player) return;

    ACPPPlayer* ExclusivePlayer = Cast<ACPPPlayer>(Player);
    if (!ExclusivePlayer) return;

    ACPPPlayerState* State = Cast<ACPPPlayerState>(ExclusivePlayer->GetPlayerState());
    if (!State) return;
    State->AddScore(Score);

    // �X�R�A�\��
    ShowScore(ExclusivePlayer);
}

void ACPPGameModeBase::ShowEvaluation(ACPPPlayer* Player, EJumpingEvaluation Evaluation)
{

}

void ACPPGameModeBase::ShowScore(ACPPPlayer* Player)
{
    if (!Player) return;

    ACPPPlayerState* State = Cast<ACPPPlayerState>(Player->GetPlayerState());
    if (!State) return;

    // �X�R�A�\��
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    if (!PlayerController) return;
    ACPPGameHUD* GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
    if (GameHUD && GameHUD->GameScoreWidget) GameHUD->GameScoreWidget->SetScoreText(State->GetScore());
}

ACPPRandomStageSpawner* ACPPGameModeBase::SpawnNextStage(UWorld* World, TSubclassOf<ACPPRandomStageSpawner> StageClass, FVector Location, FVector FirstFloor)
{
    FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(
        Location,
        FRotator::ZeroRotator
    );
    ACPPRandomStageSpawner* NewStage = World->SpawnActorDeferred<ACPPRandomStageSpawner>(
        StageClass,
        SpawnTransform
    );
    NewStage->FirstFloorLocation = FirstFloor;
    NewStage->FinishSpawning(SpawnTransform);

    return NewStage;
}

ATriggerVolume* ACPPGameModeBase::SpawnNextTrigger(UWorld* World, ATriggerVolume* TrgTemplete, FVector Location)

{
    FActorSpawnParameters SpawnParam;
    SpawnParam.Template = TrgTemplete;
    TObjectPtr<ATriggerVolume> NewTriggerVolume =
        World->SpawnActor<ATriggerVolume>(
            Location - TrgTemplete->GetActorLocation(), // �e���v���[�g�̕�������
            FRotator::ZeroRotator,
            SpawnParam
        );

    return NewTriggerVolume;
}

ATargetPoint* ACPPGameModeBase::SpawnNextStageMakingPoint(UWorld* World, FVector Location)
{
    return World->SpawnActor<ATargetPoint>(
        Location,
        FRotator::ZeroRotator,
        FActorSpawnParameters()
    );
}
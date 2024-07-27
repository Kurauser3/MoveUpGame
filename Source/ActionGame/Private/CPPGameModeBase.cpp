// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPGameModeBase.h"
#include "CPPRandomStageSpawner.h"
#include "CPPMagma.h"
#include "CPPPlayer.h"
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
    UE_LOG(LogTemp, Log, TEXT("MyLog: GameModeConstructorBegin"));
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Developers/tkr31/Characters/BP_Player"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    UE_LOG(LogTemp, Log, TEXT("MyLog: GameModeConstructorEnd"));

}

void ACPPGameModeBase::BeginPlay()
{
    UE_LOG(LogTemp, Log, TEXT("MyLog: GameModeBeginPlayBegin"));

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
    }

    // �}�O�}�ɓ���������Q�[���I�[�o�[ +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // �{���̓L�����N�^�[�Ƀq�b�g�A���邢�̓I�[�o�[���b�v�������ɍ쓮���Ăق������A�L�����N�^�[�������Ă��Ȃ����Ɍ����Ȃ������̂ŕۗ�
    /*
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

    UE_LOG(LogTemp, Log, TEXT("MyLog: GameModeBeginPlayEnd"));

}

void ACPPGameModeBase::HandleOverlapSpawnTrigger(AActor* OverlappedActor, AActor* OtherActor)
{
    SpawnNext(OverlappedActor, OtherActor);
    IncreaseMagmaSpeed();
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

    Progress++; // �X�e�[�W�i��+
    APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
    if (PlayerController)
    {
        // �X�R�A�\��
        ACPPGameHUD* GameHUD = Cast<ACPPGameHUD>(PlayerController->GetHUD());
        if (GameHUD && GameHUD->GameScoreWidget) GameHUD->GameScoreWidget->SetScoreText(100 * Progress);
    }
}

void ACPPGameModeBase::IncreaseMagmaSpeed()
{
    TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPMagma::StaticClass());
    TObjectPtr<ACPPMagma> Magma = Cast<ACPPMagma>(ActorToFind);
    if (!Magma) return;

    Magma->Speed = Magma->BaseSpeed + Progress;
    if (Magma->Speed <= MagmaMaxSpeed) return;
    Magma->Speed = MagmaMaxSpeed;
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
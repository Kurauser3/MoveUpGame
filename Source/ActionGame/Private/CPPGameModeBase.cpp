// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPGameModeBase.h"
#include "CPPRandomStageSpawner.h"
#include "CPPPlayer.h"

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
			TriggerVolumeCast->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::SpawnNext);
	}

}

void ACPPGameModeBase::SpawnNext(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!Stage) return; // ��������X�e�[�W���ݒ肳��Ă��Ȃ���Ζ���

	if (!Cast<ACharacter>(OtherActor)) return; // �I�[�o�[���b�v�����̂��L�����N�^�[�łȂ���Ζ���

	TObjectPtr<ATriggerVolume> TriggerVolume = Cast<ATriggerVolume>(OverlappedActor);
	if (!TriggerVolume) return; // ���Ό����g���K�[�{�����[���łȂ���Ζ���

	TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ATargetPoint::StaticClass());
	TObjectPtr<ATargetPoint> TargetPoint = Cast<ATargetPoint>(ActorToFind);
	if (!TargetPoint) return; // ���̐����|�C���g��������Ȃ���Ζ���
	
	// ���̃g���K�[�{�����[���𐶐�����
	ATriggerVolume* NewTriggerVolume = SpawnNextTrigger(GetWorld(), TriggerVolume, TargetPoint->GetActorLocation());
	// ���̃g���K�[�ł����l�̏���������悤�ɂ���
	NewTriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::SpawnNext);

	// �X�e�[�W����
	PrevStage = SpawnNextStage(GetWorld(), Stage, TargetPoint->GetActorLocation(), PrevStage->NextFloorLocation);

	// ���̃X�|�[���n�_�𐶐�����
	SpawnNextStageMakingPoint(
		GetWorld(),
		TargetPoint->GetActorLocation() + (TargetPoint->GetActorLocation() - TriggerVolume->GetActorLocation())
	);
	// �s�v�ɂȂ����A�N�^�͏����Ă���
	TriggerVolume->Destroy();
	TargetPoint->Destroy();
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
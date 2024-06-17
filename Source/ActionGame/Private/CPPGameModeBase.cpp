// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPGameModeBase.h"
#include "CPPRandomStageSpawner.h"
#include "CPPPlayer.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include <Kismet/KismetSystemLibrary.h>
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

	// �ŏ��̃X�e�[�W�X�|�i�[��ݒ肷��
	TArray<TObjectPtr<AActor>> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(this, ATriggerVolume::StaticClass(), ActorsToFind);

	for (TObjectPtr<AActor> TriggerVolume : ActorsToFind)
	{
		TObjectPtr<ATriggerVolume> TriggerVolumeCast = Cast<ATriggerVolume>(TriggerVolume);
		if (TriggerVolumeCast)
		{
			TriggerVolumeCast->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::SpawnNextTrigger);
		}
	}

}

void ACPPGameModeBase::SpawnNextTrigger(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!Stage) return; // ��������X�e�[�W���ݒ肳��Ă��Ȃ���Ζ���

	if (!Cast<ACharacter>(OtherActor)) return; // �I�[�o�[���b�v�����̂��L�����N�^�[�łȂ���Ζ���

	TObjectPtr<ATriggerVolume> TriggerVolume = Cast<ATriggerVolume>(OverlappedActor);
	if (!TriggerVolume) return; // ���Ό����g���K�[�{�����[���łȂ���Ζ���

	TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ATargetPoint::StaticClass());
	TObjectPtr<ATargetPoint> TargetPoint = Cast<ATargetPoint>(ActorToFind);
	if (!TargetPoint) return; // ���̐����|�C���g��������Ȃ���Ζ���
	
	// ���̃g���K�[�{�����[���𐶐�����
	FActorSpawnParameters SpawnParam;
	SpawnParam.Template = TriggerVolume;
	TObjectPtr<ATriggerVolume> NewTriggerVolume = 
		GetWorld()->SpawnActor<ATriggerVolume>(
			TargetPoint->GetActorLocation() - TriggerVolume->GetActorLocation(), // �e���v���[�g�̕�������
			TargetPoint->GetActorRotation() - TriggerVolume->GetActorRotation(),
			SpawnParam
		);
	// ���̃g���K�[�ł����l�̏���������悤�ɂ���
	NewTriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::SpawnNextTrigger);

	// �X�e�[�W����
	GetWorld()->SpawnActor<ACPPRandomStageSpawner>(
		Stage,
		TargetPoint->GetActorLocation(),
		TargetPoint->GetActorRotation(),
		FActorSpawnParameters()
	);

	// ���̃X�|�[���n�_�𐶐�����
	GetWorld()->SpawnActor<ATargetPoint>(
		TargetPoint->GetActorLocation() + (TargetPoint->GetActorLocation() - TriggerVolume->GetActorLocation()),
		TargetPoint->GetActorRotation(),
		FActorSpawnParameters()
	);

	TriggerVolume->Destroy(); // ���Ό��͔j�����čĔ��΂�h��
	TargetPoint->Destroy();

	UKismetSystemLibrary::PrintString(this, FString("Triggered"));
}
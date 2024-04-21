// Fill out your copyright notice in the Description page of Project Settings.


#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/TriggerVolume.h>
#include <Engine/TargetPoint.h>


#include "CPPGameModeBase.h"
#include "CPPPlayer.h"

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
	if (!Cast<ACharacter>(OtherActor)) return; // オーバーラップしたのがキャラクターでなければ無視

	TObjectPtr<ATriggerVolume> TriggerVolume = Cast<ATriggerVolume>(OverlappedActor);
	if (!TriggerVolume) return; // 発火元がトリガーボリュームでなければ無視

	TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ATargetPoint::StaticClass());
	TObjectPtr<ATargetPoint> TargetPoint = Cast<ATargetPoint>(ActorToFind);
	if (!TargetPoint) return; // 次の生成ポイントが見つからなければ無視
	
	// 次のトリガーボリュームを生成する
	FActorSpawnParameters SpawnParam;
	SpawnParam.Template = TriggerVolume;
	TObjectPtr<ATriggerVolume> NewTriggerVolume = 
		GetWorld()->SpawnActor<ATriggerVolume>(
			TargetPoint->GetActorLocation(), 
			TargetPoint->GetActorRotation(), 
			SpawnParam
		);
	// 次のトリガーでも同様の処理が走るようにする
	NewTriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::SpawnNextTrigger);

	// 次のスポーン地点を生成する
	GetWorld()->SpawnActor<ATargetPoint>(
		TargetPoint->GetActorLocation()*2 - TriggerVolume->GetActorLocation(),
		TargetPoint->GetActorRotation()*2 - TriggerVolume->GetActorRotation(),
		FActorSpawnParameters()
	);

	TriggerVolume->Destroy(); // 発火元は破棄して再発火を防ぐ
	TargetPoint->Destroy();

	UKismetSystemLibrary::PrintString(this, FString("Triggered"));
}
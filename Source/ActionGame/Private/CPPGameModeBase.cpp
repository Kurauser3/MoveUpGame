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
	// 他のUIから遷移してきた場合はマウス操作をゲームに戻す必要があるのでここで設定
	TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
		Controller->SetShowMouseCursor(false);
	}
	
	TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPRandomStageSpawner::StaticClass());
	PrevStage = Cast<ACPPRandomStageSpawner>(ActorToFind);
	if (!PrevStage) return;

	// 最初のステージスポナーを設定する
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
	if (!Stage) return; // 生成するステージが設定されていなければ無視

	if (!Cast<ACharacter>(OtherActor)) return; // オーバーラップしたのがキャラクターでなければ無視

	TObjectPtr<ATriggerVolume> TriggerVolume = Cast<ATriggerVolume>(OverlappedActor);
	if (!TriggerVolume) return; // 発火元がトリガーボリュームでなければ無視

	TObjectPtr<AActor> ActorToFind = UGameplayStatics::GetActorOfClass(this, ATargetPoint::StaticClass());
	TObjectPtr<ATargetPoint> TargetPoint = Cast<ATargetPoint>(ActorToFind);
	if (!TargetPoint) return; // 次の生成ポイントが見つからなければ無視
	
	// 次のトリガーボリュームを生成する
	ATriggerVolume* NewTriggerVolume = SpawnNextTrigger(GetWorld(), TriggerVolume, TargetPoint->GetActorLocation());
	// 次のトリガーでも同様の処理が走るようにする
	NewTriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ACPPGameModeBase::SpawnNext);

	// ステージ生成
	PrevStage = SpawnNextStage(GetWorld(), Stage, TargetPoint->GetActorLocation(), PrevStage->NextFloorLocation);

	// 次のスポーン地点を生成する
	SpawnNextStageMakingPoint(
		GetWorld(),
		TargetPoint->GetActorLocation() + (TargetPoint->GetActorLocation() - TriggerVolume->GetActorLocation())
	);
	// 不要になったアクタは消しておく
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
			Location - TrgTemplete->GetActorLocation(), // テンプレートの分を除く
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
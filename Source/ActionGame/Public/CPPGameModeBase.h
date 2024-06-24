// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ACPPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	class ACPPRandomStageSpawner* PrevStage;

public:
	ACPPGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TSubclassOf<ACPPRandomStageSpawner> Stage;
	
protected:
	virtual void BeginPlay() override;

	/**
	 * 次のステージを生成するためのTriggerVolumeとTargetPointを生成する。
	 * 発火元のTriggerVolumeは破棄する。
	 */
	UFUNCTION()
	void SpawnNext(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	static ACPPRandomStageSpawner* SpawnNextStage(UWorld* World, TSubclassOf<ACPPRandomStageSpawner> StageClass, FVector Location, FVector FirstFloor = FVector(-1, -1, -1));

	UFUNCTION()
	static ATriggerVolume* SpawnNextTrigger(UWorld* World, ATriggerVolume* TrgTemplete, FVector Location);

	UFUNCTION()
	static ATargetPoint* SpawnNextStageMakingPoint(UWorld* World, FVector Location);
	/*
	*/

};

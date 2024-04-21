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

public:
	ACPPGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TSubclassOf< class ACPPRandomStageSpawner > Stage;
	
protected:
	virtual void BeginPlay() override;

	/**
	 * 次のステージを生成するためのTriggerVolumeとTargetPointを生成する。
	 * 発火元のTriggerVolumeは破棄する。
	 */
	UFUNCTION()
	void SpawnNextTrigger(AActor* OverlappedActor, AActor* OtherActor);
};

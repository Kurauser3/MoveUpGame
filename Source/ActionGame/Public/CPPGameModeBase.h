// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPPEJumpEvaluation.h"

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
	/* ステージ生成でひとつ前のステージの状況を引き継ぐための状態 */
	class ACPPRandomStageSpawner* PrevStage;

	UPROPERTY(VisibleAnywhere)
	bool bChargingProgressShown = false;

	UPROPERTY(VisibleAnywhere)
	TMap <TEnumAsByte<EJumpingEvaluation>, float> JumpScoreMap = {
		{EJumpingEvaluation::EVAL_Perfect, 250.f},
		{EJumpingEvaluation::EVAL_Good, 150.f},
		{EJumpingEvaluation::EVAL_Bad, 50.f}
	};

public:
	ACPPGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TSubclassOf<ACPPRandomStageSpawner> Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagmaMaxSpeed = 80.f;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlapSpawnTrigger(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void HandleOverlapMagma(AActor* OverlappedActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HandleOverlapMagmaTEMP(class ACPPMagma* Magma, class ACPPPlayer* Player);

	UFUNCTION()
	void HandleCharacterCharging(ACPPPlayer* Player, float JumpVelocity, float Min, float Max);

	UFUNCTION()
	void HandleCharacterJump(ACPPPlayer* Player);

	UFUNCTION()
	void HandleCharacterLanding(ACPPPlayer* Player, float FallingTime);

	UFUNCTION()
	/* ゲームがステージ生成を行うための一通りの手続き */
	void SpawnNext(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	/* マグマの上昇スピードを上げる */
	void IncreaseMagmaSpeed(uint16 increment);

	UFUNCTION()
	static void IncreaseProgress(ACharacter* Player);

	UFUNCTION()
	static EJumpingEvaluation EvaluateJumping(float FallingTime);

	UFUNCTION()
	static void IncreaseJumpScore(ACharacter* Player, float Score);

	UFUNCTION()
	static void ShowScore(ACPPPlayer* Player);

	UFUNCTION()
	/* 次のステージを生成 */
	static ACPPRandomStageSpawner* SpawnNextStage(UWorld* World, TSubclassOf<ACPPRandomStageSpawner> StageClass, FVector Location, FVector FirstFloor = FVector(-1, -1, -1));

	UFUNCTION()
	/* 次のステージ生成トリガーを作る */
	static ATriggerVolume* SpawnNextTrigger(UWorld* World, ATriggerVolume* TrgTemplete, FVector Location);

	UFUNCTION()
	/* 次のステージ生成座標を作る */
	static ATargetPoint* SpawnNextStageMakingPoint(UWorld* World, FVector Location);

};

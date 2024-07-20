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
	/* �X�e�[�W�����łЂƂO�̃X�e�[�W�̏󋵂������p�����߂̏�� */
	class ACPPRandomStageSpawner* PrevStage;

	UPROPERTY(VisibleAnywhere)
	/* �X�e�[�W�̐i���B�X�e�[�W����������閈�Ɉ�オ�� */
	uint32 Progress = 0;

	UPROPERTY(VisibleAnywhere)
	bool bChargingProgressShown = false;

public:
	ACPPGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TSubclassOf<ACPPRandomStageSpawner> Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagmaMaxSpeed = 70.f;
	
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
	/* �Q�[�����X�e�[�W�������s�����߂̈�ʂ�̎葱�� */
	void SpawnNext(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	/* �}�O�}�̏㏸�X�s�[�h���グ�� */
	void IncreaseMagmaSpeed();

	UFUNCTION()
	/* ���̃X�e�[�W�𐶐� */
	static ACPPRandomStageSpawner* SpawnNextStage(UWorld* World, TSubclassOf<ACPPRandomStageSpawner> StageClass, FVector Location, FVector FirstFloor = FVector(-1, -1, -1));

	UFUNCTION()
	/* ���̃X�e�[�W�����g���K�[����� */
	static ATriggerVolume* SpawnNextTrigger(UWorld* World, ATriggerVolume* TrgTemplete, FVector Location);

	UFUNCTION()
	/* ���̃X�e�[�W�������W����� */
	static ATargetPoint* SpawnNextStageMakingPoint(UWorld* World, FVector Location);

};

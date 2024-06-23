// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/BoxComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPRandomStageSpawner.generated.h"

UCLASS()
class ACTIONGAME_API ACPPRandomStageSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FVector AreaMaxVector;

	UPROPERTY(VisibleAnywhere)
	FVector AreaMinVector;
	
public:	
	// Sets default values for this actor's properties
	ACPPRandomStageSpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> StageWall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UBoxComponent> Area;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StageFloorClass")
	TSubclassOf<class ACPPStageFloor> Floor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Limitation")
	uint8 NumberOfFloor = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limitation")
	double MaxDistance = 250.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limitation")
	double MinHeight = 70.0;

	/**
	 * �ŏ��ɃX�|�[�������t���A�̈ʒu�B
	 * �G���A�̒[(�e���̒l���ŏ��ɂȂ���W)����̑��ΓI�Ȉʒu�w��B
	 * XY�������̒l�Ȃ炻���̓����_���ɂȂ�AZ�������̒l�Ȃ�X�e�[�W�̍ŉ����ɐ��������
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limitation")
	FVector FirstFloorLocation = FVector(-1, -1, -1);

	/**
	 * ���ɃX�|�[�������t���A�̈ʒu�B
	 * ���̃G���A�̌�Ɍq����`�œ����G���A���X�|�[������ꍇ�A
	 * ���̃G���A��FirstFloorLocation�ɃZ�b�g���Ēn�����̃G���A�̂悤�Ɍ����邱�Ƃ��ł���B
	 */
	UPROPERTY(VisibleAnywhere)
	FVector NextFloorLocation = FVector(-1, -1, -1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void MakeFirstFloorLocation();

	UFUNCTION()
	void MakeNextFloorLocation();

	UFUNCTION()
	void SpawnStage();

	UFUNCTION()
	static void SpawnFloor(FVector Location, FVector Max, FVector Min, TSubclassOf<ACPPStageFloor> Actor, UWorld* World);

	UFUNCTION()
	static bool ValidateLocation(FVector Location, FVector Max, FVector Min);

	UFUNCTION()
	static double MakeFirstRandom(double Coordinate, double Max, double Min);

	UFUNCTION()
	static double MakeFirstX(double Coordinate, double Max, double Min);

	UFUNCTION()
	static double MakeFirstY(double Coordinate, double Max, double Min);

	UFUNCTION()
	static double MakeFirstZ(double Coordinate, double Max, double Min);

	UFUNCTION()
	static double MakeRandom(double Max, double Min);

	UFUNCTION()
	static double GetDistanceSomeOfXYZ(double Distance, double OneOfXYZ1, double OneOfXYZ2);

	UFUNCTION()
	static double GetMaxX(double Distance, double Y, double Z, FVector Prev = FVector::ZeroVector);

	UFUNCTION()
	static double GetMinX(double Distance, double Y, double Z, FVector Prev = FVector::ZeroVector);

	UFUNCTION()
	static double GetMaxY(double Distance, double X, double Z, FVector Prev = FVector::ZeroVector);

	UFUNCTION()
	static double GetMinY(double Distance, double X, double Z, FVector Prev = FVector::ZeroVector);

	UFUNCTION()
	static FVector GetFloorBounds(ACPPStageFloor* Actor);
};


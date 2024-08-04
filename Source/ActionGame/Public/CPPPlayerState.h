// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ACPPPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	/* ステージの進捗。ステージが生成される毎に一つ上がる */
	uint32 Progress = 0;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScoreIncreamentByProgress = 1000.f;

	uint32 GetProgress();

	void SetProgress(uint32 Progress);

	void AddScore(float Increament);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPGameStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCPPGameStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetScoreText(uint32 Score);

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreText;

};

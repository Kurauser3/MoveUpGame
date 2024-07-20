// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPPlayerStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCPPPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetChargingProgress(float Progress, float Max, float Min);
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ChargeBar;
};

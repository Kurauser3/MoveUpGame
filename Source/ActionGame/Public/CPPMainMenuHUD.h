// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CPPMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ACPPMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf< UUserWidget > MainMenuWidgetClass;

protected:

	virtual void BeginPlay() override;
	
};

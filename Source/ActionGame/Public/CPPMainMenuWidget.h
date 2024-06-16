// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPMainMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCPPMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Play;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Options;

	UFUNCTION()
	void OnPlayClicked();
	
};

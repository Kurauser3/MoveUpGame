#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CPPGameHUD.generated.h"

UCLASS()
class ACTIONGAME_API ACPPGameHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf< UUserWidget > GameOverWidgetClass;

	UFUNCTION()
	void ShowGameOver();

};

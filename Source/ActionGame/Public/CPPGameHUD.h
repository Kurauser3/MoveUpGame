#pragma once

#include "CPPGameStateWidget.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf< UUserWidget > GameScoreWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf< UUserWidget > CharacterStateWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf< UUserWidget > GameHelpWidgetClass;

	UPROPERTY(VisibleAnywhere)
    TObjectPtr< UCPPGameStateWidget > GameScoreWidget;

	UFUNCTION()
	void ShowGameOver();

	UFUNCTION()
	void ShowGameScore();

	UFUNCTION()
	void ShowCharacterState();

	UFUNCTION()
	void ShowGameHelp();

protected:

	virtual void BeginPlay() override;

};

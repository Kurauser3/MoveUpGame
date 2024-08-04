#pragma once

#include "CPPGameStateWidget.h"
#include "CPPPlayerStateWidget.h"
#include "CPPEJumpEvaluation.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf< UUserWidget > EvaluationWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCPPEvaluationWidget* EvaluationWidget;

	UPROPERTY(VisibleAnywhere)
    TObjectPtr< UCPPGameStateWidget > GameScoreWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr< UCPPPlayerStateWidget > PlayerStateWidget;

	UFUNCTION()
	void ShowGameOver();

	UFUNCTION()
	void ShowGameScore();

	UFUNCTION()
	void ShowCharacterState();

	UFUNCTION()
	void RemoveCharacterState();

	UFUNCTION()
	void ShowGameHelp();

	UFUNCTION()
	void ShowEvaluation();

	UFUNCTION()
	void SetEvaluation(EJumpingEvaluation Evaluation);

protected:

	virtual void BeginPlay() override;

};

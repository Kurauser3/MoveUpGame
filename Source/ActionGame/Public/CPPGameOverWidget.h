#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPGameOverWidget.generated.h"

class UButton;

UCLASS()
class ACTIONGAME_API UCPPGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> MainManu;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> ReloadLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Restart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Title;

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnTitleClicked();
};

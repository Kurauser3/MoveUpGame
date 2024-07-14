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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Restart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Title;

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnTitleClicked();
};

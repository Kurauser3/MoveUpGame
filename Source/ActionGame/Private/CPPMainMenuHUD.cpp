#include "CPPMainMenuHUD.h"

#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void ACPPMainMenuHUD::BeginPlay()
{
    TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(this, 0);

    if (MainMenuWidgetClass && Controller)
    {
        TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, MainMenuWidgetClass, Controller);

        Widget->AddToViewport();

        UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Controller, Widget);
        Controller->SetShowMouseCursor(true);
    }
}
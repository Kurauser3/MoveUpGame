#include "CPPGameHUD.h"

#include <Blueprint/WidgetBlueprintLibrary.h>


void ACPPGameHUD::ShowGameOver()
{
    if (!PlayerOwner) return;
    if (!GameOverWidgetClass) return;

    TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, GameOverWidgetClass, PlayerOwner);
    Widget->AddToViewport();

    UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerOwner, Widget);
    PlayerOwner->SetShowMouseCursor(true);
}
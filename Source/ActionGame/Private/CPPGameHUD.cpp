#include "CPPGameHUD.h"

#include <Blueprint/WidgetBlueprintLibrary.h>

void ACPPGameHUD::BeginPlay()
{
    ShowGameHelp();
}

void ACPPGameHUD::ShowGameOver()
{
    if (!PlayerOwner) return;
    if (!GameOverWidgetClass) return;

    TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, GameOverWidgetClass, PlayerOwner);
    Widget->AddToViewport();

    UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerOwner, Widget);
    PlayerOwner->SetShowMouseCursor(true);
}

void ACPPGameHUD::ShowGameScore()
{

}

void ACPPGameHUD::ShowCharacterState()
{

}

void ACPPGameHUD::ShowGameHelp()
{
    if (!PlayerOwner) return;
    if (!GameOverWidgetClass) return;

    TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, GameHelpWidgetClass, PlayerOwner);
    Widget->AddToViewport();
}
#include "CPPGameHUD.h"

#include <Blueprint/WidgetBlueprintLibrary.h>

void ACPPGameHUD::BeginPlay()
{
    ShowGameHelp();
    ShowGameScore();
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
    if (!PlayerOwner) return;
    if (!GameOverWidgetClass) return;

    TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, GameScoreWidgetClass, PlayerOwner);
    GameScoreWidget = Cast<UCPPGameStateWidget>(Widget);
    Widget->AddToViewport();
}

void ACPPGameHUD::ShowCharacterState()
{
    if (!PlayerOwner) return;
    if (!CharacterStateWidgetClass) return;

    TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, CharacterStateWidgetClass, PlayerOwner);
    PlayerStateWidget = Cast<UCPPPlayerStateWidget>(Widget);
    Widget->AddToViewport();
}



void ACPPGameHUD::RemoveCharacterState()
{
    if (!PlayerStateWidget) return;
    PlayerStateWidget->RemoveFromViewport();
    PlayerStateWidget = nullptr;
}

void ACPPGameHUD::ShowGameHelp()
{
    if (!PlayerOwner) return;
    if (!GameOverWidgetClass) return;

    TObjectPtr<UUserWidget> Widget = UWidgetBlueprintLibrary::Create(this, GameHelpWidgetClass, PlayerOwner);
    Widget->AddToViewport();
}
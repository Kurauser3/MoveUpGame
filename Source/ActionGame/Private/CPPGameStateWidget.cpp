// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPGameStateWidget.h"

#include <Components/TextBlock.h>

void UCPPGameStateWidget::SetScoreText(uint32 Score)
{
    if (!ScoreText) return;

    FText Text = FText::FromString(FString::Printf(TEXT("Score : %d"), Score));
    ScoreText->SetText(Text);
}
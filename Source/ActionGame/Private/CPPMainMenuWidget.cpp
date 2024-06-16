// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "CPPMainMenuWidget.h"

void UCPPMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Play->OnClicked.AddUniqueDynamic(this, &UCPPMainMenuWidget::OnPlayClicked);
}

void UCPPMainMenuWidget::OnPlayClicked()
{
    UGameplayStatics::OpenLevel(this, FName(TEXT("MoveUp")));
}
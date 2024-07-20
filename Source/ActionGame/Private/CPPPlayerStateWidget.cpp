// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPPlayerStateWidget.h"

#include <Components/ProgressBar.h>

void UCPPPlayerStateWidget::SetChargingProgress(float Progress, float Max, float Min)
{
    if (Max < Min) return;
    float P = Progress < Min ? Min : Progress;
    ChargeBar->SetPercent((P - Min) / (Max - Min));
}
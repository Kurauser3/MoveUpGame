// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPEvaluationWidget.h"

#include <Components/TextBlock.h>

void UCPPEvaluationWidget::SetEvaluation(EJumpingEvaluation Eval)
{
    if (!Evaluation) return;

    FText Txt = FText::FromString(EvaluationTextMap[Eval]);
    Evaluation->SetText(Txt);
    Evaluation->SetColorAndOpacity(FSlateColor(EvaluationColorMap[Eval]));
    // アニメーションの開始
    PlayAnimation(EvaluationAnim);
}
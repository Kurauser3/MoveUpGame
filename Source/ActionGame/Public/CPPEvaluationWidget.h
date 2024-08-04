// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CPPEJumpEvaluation.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPEvaluationWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCPPEvaluationWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EJumpingEvaluation>, FString> EvaluationTextMap = {
		{EJumpingEvaluation::EVAL_Perfect, FString(TEXT("Perfect"))},
		{EJumpingEvaluation::EVAL_Good, FString(TEXT("Good"))},
		{EJumpingEvaluation::EVAL_Bad, FString(TEXT("Bad"))},
	};

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EJumpingEvaluation>, FColor> EvaluationColorMap = {
		{EJumpingEvaluation::EVAL_Perfect, FColor::Cyan},
		{EJumpingEvaluation::EVAL_Good, FColor::Green},
		{EJumpingEvaluation::EVAL_Bad, FColor::Silver},
	};
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Evaluation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EvaluationAnim;

	UFUNCTION()
	void SetEvaluation(EJumpingEvaluation Eval);
};

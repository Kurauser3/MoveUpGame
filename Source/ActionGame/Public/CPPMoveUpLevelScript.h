// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CPPMoveUpLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ACPPMoveUpLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	float HeightStagemakeStart = 1170.0f;
};

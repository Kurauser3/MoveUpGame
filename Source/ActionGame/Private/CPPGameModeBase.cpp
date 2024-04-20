// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPGameModeBase.h"
#include "CPPPlayer.h"

ACPPGameModeBase::ACPPGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Developers/tkr31/Characters/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
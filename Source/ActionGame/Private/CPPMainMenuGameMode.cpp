// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMainMenuGameMode.h"
#include "UObject/ConstructorHelpers.h"

ACPPMainMenuGameMode::ACPPMainMenuGameMode()
{
    static ConstructorHelpers::FClassFinder<AActor> HUD(TEXT("/Game/Developers/tkr31/Menus/BP_MainMenu"));


    if (HUD.Succeeded())
    {
        HUDClass = HUD.Class;
    }
}
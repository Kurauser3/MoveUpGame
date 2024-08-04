// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPPlayerState.h"

uint32 ACPPPlayerState::GetProgress()
{
    return Progress;
}

void ACPPPlayerState::SetProgress(uint32 NewProgress)
{
    uint32 Diff = NewProgress - Progress;
    Progress = NewProgress;
    AddScore(Diff * ScoreIncreamentByProgress);
}

void ACPPPlayerState::AddScore(float Increament)
{
    SetScore(GetScore() + Increament);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPGameObserver.h"

#include <Kismet/GameplayStatics.h>

// Sets default values
ACPPGameObserver::ACPPGameObserver()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void ACPPGameObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 対象のアクタを探す +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	AActor* ActorToFind;
	if (!Magma)
	{
		ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPMagma::StaticClass());
		if (ActorToFind) Magma = Cast<ACPPMagma>(ActorToFind);
	}
	if (!Character)
	{
		ActorToFind = UGameplayStatics::GetActorOfClass(this, ACPPPlayer::StaticClass());
		if (ActorToFind) Character = Cast<ACPPPlayer>(ActorToFind);
	}

    // キャラクターとマグマの位置がZ軸で重なっていないか確認し、イベントを発行 ++++++++++++++++++++++
	if (Magma && Character)
	{
		FVector MagmaLocation =  Magma->GetActorLocation();
		FVector CharLocation;
		FVector CharExtent;
		Character->GetActorBounds(true, CharLocation, CharExtent);
		FVector CharMinLocation = CharLocation - CharExtent;

		if ((CharMinLocation.Z <= MagmaLocation.Z) && !bMagmaHitCharacter)
		{
			bMagmaHitCharacter = true;
			OnMagmaHitCharacter.Broadcast(Magma, Character); // キャラクターとマグマがヒット(あつい)
		}
		else if (CharMinLocation.Z > MagmaLocation.Z)
		{
			bMagmaHitCharacter = false; // ゲームルール上ありえないが、マグマより上に逃げ出した時は判定リセット
		}

	}

}


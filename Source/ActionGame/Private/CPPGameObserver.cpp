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
	
	// �Ώۂ̃A�N�^��T�� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

    // �L�����N�^�[�ƃ}�O�}�̈ʒu��Z���ŏd�Ȃ��Ă��Ȃ����m�F���A�C�x���g�𔭍s ++++++++++++++++++++++
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
			OnMagmaHitCharacter.Broadcast(Magma, Character); // �L�����N�^�[�ƃ}�O�}���q�b�g(����)
		}
		else if (CharMinLocation.Z > MagmaLocation.Z)
		{
			bMagmaHitCharacter = false; // �Q�[�����[���゠�肦�Ȃ����A�}�O�}����ɓ����o�������͔��胊�Z�b�g
		}

	}

}


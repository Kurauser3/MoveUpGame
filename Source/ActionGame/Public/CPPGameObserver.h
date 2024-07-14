#pragma once

#include "CPPMagma.h"
#include "CPPPlayer.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPGameObserver.generated.h"

/*
 * ISSUE
 * 
 * �L�����N�^�[�������Ă��Ȃ����Ƀ}�O�}�Ƃ̃q�b�g���肪�ǂ����Ă����܂��������A
 * �a�X�����A���ꂼ��̃A�N�^�̈ʒu���Ď����ďd�Ȃ����Ƃ��ɒʒm����N���X��������B
 * UnrealEngine��Collision�ɔs�k����...(��)
 * 
 * TODO : ��ł�����xHit����Ɋւ��X�e�[�g���������Ă��̃A�N�^������
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMagmaHitCharacterSignature, ACPPMagma*, Magma, ACPPPlayer*, Player);

UCLASS()
class ACTIONGAME_API ACPPGameObserver : public AActor
{
	GENERATED_BODY()

	ACPPMagma* Magma;
	
	ACPPPlayer* Character;
	
public:	
	// Sets default values for this actor's properties
	ACPPGameObserver();

	UPROPERTY(VisibleAnywhere)
	bool bMagmaHitCharacter = false;

	FMagmaHitCharacterSignature OnMagmaHitCharacter;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


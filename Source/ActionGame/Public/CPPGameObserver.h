#pragma once

#include "CPPMagma.h"
#include "CPPPlayer.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPGameObserver.generated.h"

/*
 * ISSUE
 * 
 * キャラクターが動いていない時にマグマとのヒット判定がどうしてもうまくいかず、
 * 渋々だが、それぞれのアクタの位置を監視して重なったときに通知するクラスを作った。
 * UnrealEngineのCollisionに敗北した...(泣)
 * 
 * TODO : 後でもう一度Hit判定に関わるステートを見直してこのアクタを消す
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


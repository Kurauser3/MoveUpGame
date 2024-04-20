// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VanishingFloorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UVanishingFloorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVanishingFloorComponent();

	// 消えるまでの時間
	UPROPERTY(EditAnywhere)
	float Lifetime = 2.0f;

	// 点滅の間隔
	UPROPERTY(EditAnywhere)
	float FlashInterval = 0.2f;

	// マテリアルを点滅させる時の色
	UPROPERTY(EditAnywhere)
	FLinearColor FlashColor = FLinearColor::Yellow;

	// 上からのコリジョンを検知した後のTick処理をデリゲートが持つようにする
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTickAfterSomethingHit, float, DeltaTime, FTimerHandle&, HandleLifetimePast);
	UPROPERTY(EditAnywhere)
	FTickAfterSomethingHit TickAfterSomethingHit;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// OnComponentHitイベントのハンドラ
	UFUNCTION(BlueprintAuthorityOnly)
	void HandleComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// コリジョンを検知した後のデフォルトのTick処理(マテリアルを点滅させる)
	UFUNCTION(BlueprintAuthorityOnly)
	void FlickerMaterial(float DeltaTime, FTimerHandle& HandleLiftimePast);

	// アクタが消滅する時に実行されるデリゲートとそのハンドル
	FTimerDelegate OnLifetimePast;
	FTimerHandle HandleLifetimePast;

	FLinearColor NextColor;
	
	// 親のスタティックメッシュコンポーネント
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	// 点滅を表現するために使用するマテリアル
	UMaterialInstanceDynamic* DynamicMaterial;

	// 点滅が切り替わってからの経過時間
	float TimeFromColorSwitched = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

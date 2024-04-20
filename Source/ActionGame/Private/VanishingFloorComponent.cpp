// Fill out your copyright notice in the Description page of Project Settings.


#include "VanishingFloorComponent.h"

// Sets default values for this component's properties
UVanishingFloorComponent::UVanishingFloorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// メンバ初期化
	NextColor = FlashColor;
}


// Called when the game starts
void UVanishingFloorComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), nullptr);
	// 親のメッシュ(のマテリアル)を動的に変更できるようにする
	StaticMesh->SetMaterial(0, DynamicMaterial);

	// イベント処理の設定
	StaticMesh->OnComponentHit.AddDynamic(this, &UVanishingFloorComponent::HandleComponentHit);
	TickAfterSomethingHit.AddDynamic(this, &UVanishingFloorComponent::FlickerMaterial);

}

void UVanishingFloorComponent::HandleComponentHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{

	if (!HandleLifetimePast.IsValid() && Hit.Normal.Z < 0)
	{
		// 上から当たり判定があった(乗られた)場合、一定時間後に消滅する
		OnLifetimePast.BindLambda([this] {
				GetOwner()->GetWorldTimerManager().ClearTimer(HandleLifetimePast);
				GetOwner()->Destroy();
			});
		GetOwner()->GetWorldTimerManager().SetTimer(HandleLifetimePast, OnLifetimePast, Lifetime, false);
	}

}

void UVanishingFloorComponent::FlickerMaterial(float DeltaTime, FTimerHandle& HandleLiftimePast)
{
	TimeFromColorSwitched += DeltaTime;

	if (TimeFromColorSwitched >= FlashInterval) {
		// 点滅の切り替わり
		TimeFromColorSwitched -= FlashInterval;
		FLinearColor PrevColor = DynamicMaterial->K2_GetVectorParameterValue(TEXT("Color"));
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), NextColor);
		NextColor = PrevColor;
	}

}

// Called every frame
void UVanishingFloorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HandleLifetimePast.IsValid()) {
		// 消えるまでのカウントダウンが始まっている場合の処理
		TickAfterSomethingHit.Broadcast(DeltaTime, HandleLifetimePast);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "VanishingFloorComponent.h"

// Sets default values for this component's properties
UVanishingFloorComponent::UVanishingFloorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// �����o������
	NextColor = FlashColor;
}


// Called when the game starts
void UVanishingFloorComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), nullptr);
	// �e�̃��b�V��(�̃}�e���A��)�𓮓I�ɕύX�ł���悤�ɂ���
	StaticMesh->SetMaterial(0, DynamicMaterial);

	// �C�x���g�����̐ݒ�
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
		// �ォ�瓖���蔻�肪������(���ꂽ)�ꍇ�A��莞�Ԍ�ɏ��ł���
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
		// �_�ł̐؂�ւ��
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
		// ������܂ł̃J�E���g�_�E�����n�܂��Ă���ꍇ�̏���
		TickAfterSomethingHit.Broadcast(DeltaTime, HandleLifetimePast);
	}
}


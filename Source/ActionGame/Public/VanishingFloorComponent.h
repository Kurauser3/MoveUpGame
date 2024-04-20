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

	// ������܂ł̎���
	UPROPERTY(EditAnywhere)
	float Lifetime = 2.0f;

	// �_�ł̊Ԋu
	UPROPERTY(EditAnywhere)
	float FlashInterval = 0.2f;

	// �}�e���A����_�ł����鎞�̐F
	UPROPERTY(EditAnywhere)
	FLinearColor FlashColor = FLinearColor::Yellow;

	// �ォ��̃R���W���������m�������Tick�������f���Q�[�g�����悤�ɂ���
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTickAfterSomethingHit, float, DeltaTime, FTimerHandle&, HandleLifetimePast);
	UPROPERTY(EditAnywhere)
	FTickAfterSomethingHit TickAfterSomethingHit;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// OnComponentHit�C�x���g�̃n���h��
	UFUNCTION(BlueprintAuthorityOnly)
	void HandleComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// �R���W���������m������̃f�t�H���g��Tick����(�}�e���A����_�ł�����)
	UFUNCTION(BlueprintAuthorityOnly)
	void FlickerMaterial(float DeltaTime, FTimerHandle& HandleLiftimePast);

	// �A�N�^�����ł��鎞�Ɏ��s�����f���Q�[�g�Ƃ��̃n���h��
	FTimerDelegate OnLifetimePast;
	FTimerHandle HandleLifetimePast;

	FLinearColor NextColor;
	
	// �e�̃X�^�e�B�b�N���b�V���R���|�[�l���g
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	// �_�ł�\�����邽�߂Ɏg�p����}�e���A��
	UMaterialInstanceDynamic* DynamicMaterial;

	// �_�ł��؂�ւ���Ă���̌o�ߎ���
	float TimeFromColorSwitched = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

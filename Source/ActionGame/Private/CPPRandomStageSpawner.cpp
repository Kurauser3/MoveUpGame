// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPRandomStageSpawner.h"
#include "CPPStageFloor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RandomStream.h"

// Entry Point ===================================================================================

// Sets default values
ACPPRandomStageSpawner::ACPPRandomStageSpawner()
{

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("SceneRoot")));
	RootComponent = SceneRoot;

	StageWall = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StageWall")));
	StageWall->SetupAttachment(RootComponent);

	Area = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Area")));
	Area->SetupAttachment(StageWall);

}


// Called when the game starts or when spawned
void ACPPRandomStageSpawner::BeginPlay()
{
	Super::BeginPlay();

	// �G���A�̃T�C�Y���擾
	AreaMaxVector = Area->GetComponentTransform().GetLocation() + Area->GetScaledBoxExtent();
	AreaMinVector = Area->GetComponentTransform().GetLocation() - Area->GetScaledBoxExtent();

	SpawnStage();
}

// Effect =========================================================================================

void ACPPRandomStageSpawner::MakeFirstFloorLocation()
{
	NextFloorLocation = FVector(
		MakeFirstX(FirstFloorLocation.X, AreaMaxVector.X, AreaMinVector.X),
		MakeFirstY(FirstFloorLocation.Y, AreaMaxVector.Y, AreaMinVector.Y),
		MakeFirstZ(FirstFloorLocation.Z, AreaMaxVector.Z, AreaMinVector.Z)
	);
}

void ACPPRandomStageSpawner::MakeNextFloorLocation()
{
	FVector PrevLocation = NextFloorLocation;
	NextFloorLocation = FVector(-1, -1, -1);

	// Z���W�������_���Ɍ��߂�
	NextFloorLocation.Z = MakeRandom(PrevLocation.Z + MaxDistance, PrevLocation.Z + MinHeight);
	// Y���W���O�̃t���A�Ɠ���AZ���W���Œ���x�̎��AX�̍ő�ŏ������߂�
	double NextXMax = GetMaxX(MaxDistance, PrevLocation.Y, NextFloorLocation.Z, PrevLocation);
	double NextXMin = GetMinX(MaxDistance, PrevLocation.Y, NextFloorLocation.Z, PrevLocation);
	// X���W�������_���Ɍ��߂�
	NextFloorLocation.X = MakeRandom(NextXMax, NextXMin);
	// �G���A�𒴂��Ȃ��悤�␳
	if (NextFloorLocation.X < 0) NextFloorLocation.X = 0;
	if (NextFloorLocation.X > (AreaMaxVector - AreaMinVector).X)
		NextFloorLocation.X = (AreaMaxVector - AreaMinVector).X;
	// XZ���W�ƍő�̋������������Ă���̂�Y���W�͈̔͂����߂�
	double NextYMax = GetMaxY(MaxDistance, NextFloorLocation.X, NextFloorLocation.Z, PrevLocation);
	double NextYMin = GetMinY(MaxDistance, NextFloorLocation.X, NextFloorLocation.Z, PrevLocation);
	// Y���W�������_���Ɍ��߂�
	NextFloorLocation.Y = MakeRandom(NextYMax, NextYMin);
	// �G���A�𒴂��Ȃ��悤�␳
	if (NextFloorLocation.Y < 0) NextFloorLocation.Y = 0;
	if (NextFloorLocation.Y > (AreaMaxVector - AreaMinVector).Y)
		NextFloorLocation.Y = (AreaMaxVector - AreaMinVector).Y;
}

void ACPPRandomStageSpawner::SpawnStage()
{
	if(!Floor) return;

	MakeFirstFloorLocation();

	// FVector Test = Area->GetComponentTransform().GetLocation();

	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Cnt: %f, %f, %f"), Test.X, Test.Y, Test.Z));
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Max: %f, %f, %f"), AreaMaxVector.X, AreaMaxVector.Y, AreaMaxVector.Z));
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Min: %f, %f, %f"), AreaMinVector.X, AreaMinVector.Y, AreaMinVector.Z));
	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("First: %f %f %f"), NextFloorLocation.X, NextFloorLocation.Y, NextFloorLocation.Z));

	uint16 SafetyCount = 0; // �������[�v���
	while (NextFloorLocation.Z < (AreaMaxVector.Z-AreaMinVector.Z) && SafetyCount < 100)
	{
		SafetyCount++;
		SpawnFloor(NextFloorLocation, AreaMaxVector, AreaMinVector, Floor, GetWorld());
		MakeNextFloorLocation();
		/*
		*/
	}
	/*
	*/
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Count: %d"), SafetyCount));

	// ���̃X�e�[�W�ɑ����`�ŃX�e�[�W���X�|�[�������ꍇ�́A���̒l���t���A�̏����ʒu�ɂł���
	NextFloorLocation.Z = NextFloorLocation.Z - (AreaMaxVector.Z - AreaMinVector.Z);
}

// Static =========================================================================================

double ACPPRandomStageSpawner::MakeFirstRandom(double Coordinate, double Max, double Min)
{
	if (Coordinate < 0)	return MakeRandom(0, Max-Min);
	if (Coordinate > (Max-Min)) return Max-Min;
	return Coordinate;
}


double ACPPRandomStageSpawner::MakeFirstX(double Coordinate, double Max, double Min)
{
	return MakeFirstRandom(Coordinate, Max, Min);
}

double ACPPRandomStageSpawner::MakeFirstY(double Coordinate, double Max, double Min)
{
	return MakeFirstRandom(Coordinate, Max, Min);
}

double ACPPRandomStageSpawner::MakeFirstZ(double Coordinate, double Max, double Min)
{
	if (Coordinate < 0) return 0;
	if (Coordinate > Max - Min) return Max - Min;
	return Coordinate;
}

double ACPPRandomStageSpawner::MakeRandom(double Max, double Min)
{
	FRandomStream rand;
	rand.GenerateNewSeed();
	return rand.FRandRange(FMath::RoundToZero(Max), FMath::RoundToZero(Min));
}

double ACPPRandomStageSpawner::GetDistanceSomeOfXYZ(double Distance, double OneOfXYZ1, double OneOfXYZ2)
{
	double sq = FMath::Square(Distance) - FMath::Square(OneOfXYZ1) - FMath::Square(OneOfXYZ2);
	if (sq < 0) return NAN;

	return FMath::Sqrt(sq);
}

double ACPPRandomStageSpawner::GetMaxX(double Distance, double Y, double Z, FVector Prev)
{
	return Prev.X + GetDistanceSomeOfXYZ(Distance, Y - Prev.Y, Z - Prev.Z);
}

double ACPPRandomStageSpawner::GetMinX(double Distance, double Y, double Z, FVector Prev)
{
	return Prev.X - GetDistanceSomeOfXYZ(Distance, Y - Prev.Y, Z - Prev.Z);
}

double ACPPRandomStageSpawner::GetMaxY(double Distance, double X, double Z, FVector Prev)
{
	return Prev.Y + GetDistanceSomeOfXYZ(Distance, X - Prev.X, Z - Prev.Z);
}

double ACPPRandomStageSpawner::GetMinY(double Distance, double X, double Z, FVector Prev)
{
	return Prev.Y - GetDistanceSomeOfXYZ(Distance, X - Prev.X, Z - Prev.Z);
}

void ACPPRandomStageSpawner::SpawnFloor(FVector Location, FVector Max, FVector Min, TSubclassOf<ACPPStageFloor> Actor, UWorld* World)
{
	if (!World) return;
	if (!Actor) return;
	if (!ValidateLocation(Location, Max-Min, FVector::ZeroVector)) return;
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Spawn: %f, %f, %f"), Location.X, Location.Y, Location.Z));


	FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(
		Min + Location,
		FRotator::ZeroRotator
	);
	World->SpawnActor<ACPPStageFloor>(Actor, SpawnTransform, FActorSpawnParameters());
}

bool ACPPRandomStageSpawner::ValidateLocation(FVector Location, FVector Max, FVector Min)
{

	return (
		Location.X <= Max.X &&
		Location.X >= Min.X &&
		Location.Y <= Max.Y &&
		Location.Y >= Min.Y &&
		Location.Z <= Max.Z &&
		Location.Z >= Min.Z
		);
}
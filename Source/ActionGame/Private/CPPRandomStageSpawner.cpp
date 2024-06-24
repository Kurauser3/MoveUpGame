// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPRandomStageSpawner.h"
#include "CPPStageFloor.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Math/RandomStream.h>

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

	// エリアのサイズを取得
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

	// Z座標をランダムに決める
	NextFloorLocation.Z = MakeRandom(PrevLocation.Z + MaxDistance, PrevLocation.Z + MinHeight);
	// Y座標が前のフロアと同一、Z座標が最低限度の時、Xの最大最小を求める
	double NextXMax = GetMaxX(MaxDistance, PrevLocation.Y, NextFloorLocation.Z, PrevLocation);
	double NextXMin = GetMinX(MaxDistance, PrevLocation.Y, NextFloorLocation.Z, PrevLocation);
	// X座標をランダムに決める
	NextFloorLocation.X = MakeRandom(NextXMax, NextXMin);
	// エリアを超えたら前のフロアを軸に逆向きに補正
	if (NextFloorLocation.X < 0) NextFloorLocation.X = PrevLocation.X*2 - NextFloorLocation.X;
	if (NextFloorLocation.X > (AreaMaxVector - AreaMinVector).X)
		NextFloorLocation.X = PrevLocation.X*2 - NextFloorLocation.X;
	// XZ座標と最大の距離が分かっているのでY座標の範囲を求める
	double NextYMax = GetMaxY(MaxDistance, NextFloorLocation.X, NextFloorLocation.Z, PrevLocation);
	double NextYMin = GetMinY(MaxDistance, NextFloorLocation.X, NextFloorLocation.Z, PrevLocation);
	// Y座標をランダムに決める
	NextFloorLocation.Y = MakeRandom(NextYMax, NextYMin);
	if (NextFloorLocation.Y < 0) NextFloorLocation.Y = PrevLocation.Y*2 - NextFloorLocation.Y;
	if (NextFloorLocation.Y > (AreaMaxVector - AreaMinVector).Y)
		NextFloorLocation.Y = PrevLocation.Y*2 - NextFloorLocation.Y;
}

void ACPPRandomStageSpawner::SpawnStage()
{
	if(!Floor) return;

	MakeFirstFloorLocation();

	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Cnt: %f, %f, %f"), Test.X, Test.Y, Test.Z));
	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Max: %f, %f, %f"), AreaMaxVector.X, AreaMaxVector.Y, AreaMaxVector.Z));
	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Min: %f, %f, %f"), AreaMinVector.X, AreaMinVector.Y, AreaMinVector.Z));
	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("First: %f %f %f"), NextFloorLocation.X, NextFloorLocation.Y, NextFloorLocation.Z));

	uint16 SafetyCount = 0; // 無限ループ回避
	while (NextFloorLocation.Z < (AreaMaxVector.Z-AreaMinVector.Z) && SafetyCount < 100)
	{
		SafetyCount++;
		SpawnFloor(NextFloorLocation, AreaMaxVector, AreaMinVector, Floor, GetWorld());
		MakeNextFloorLocation();
	}
	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Count: %d"), SafetyCount));

	// このステージに続く形でステージがスポーンされる場合は、この値をフロアの初期位置にできる
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
	// GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString::Printf(TEXT("Spawn: %f, %f, %f"), Location.X, Location.Y, Location.Z));

	FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(
		Min + Location,
		FRotator::ZeroRotator
	);

	ACPPStageFloor* NewFloor = World->SpawnActorDeferred<ACPPStageFloor>(Actor, SpawnTransform);
	// フロアのサイズを含めてエリア内に収まるように補正
	FVector Extent = GetFloorBounds(NewFloor);
	FVector NewLocation = SpawnTransform.GetLocation();
	if (Extent.X + NewLocation.X > Max.X) NewLocation.X -= Extent.X;
	if (Extent.Y + NewLocation.Y > Max.Y) NewLocation.Y -= Extent.Y;
	if (NewLocation.X - Extent.X < Min.X) NewLocation.X += Extent.X;
	if (NewLocation.Y - Extent.Y < Min.Y) NewLocation.Y += Extent.Y;
	SpawnTransform.SetLocation(NewLocation);

	NewFloor->FinishSpawning(SpawnTransform);
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

FVector ACPPRandomStageSpawner::GetFloorBounds(ACPPStageFloor* Actor)
{
	if (!Actor) return FVector::ZeroVector;

	FVector Origin;
	FVector Extent;
	Actor->GetActorBounds(true, Origin, Extent);

	return Extent;
}
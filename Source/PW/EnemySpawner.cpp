#include "EnemySpawner.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxEnemies = 10;
	InitialEnemyCount = 5;
	ActiveEnemyCount = 0;
	SpawnAreaExtent = FVector(500.0f, 500.0f, 0.0f);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 초기 적 생성
	for (int32 i = 0; i < InitialEnemyCount; i++)
	{
		FVector SpawnLocation = GetRandomSpawnLocation();
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f); // X축 고정 방향
		SpawnEnemy(SpawnLocation, SpawnRotation);
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AEnemy* AEnemySpawner::SpawnEnemy(FVector Location, FRotator Rotation)
{
	if (ActiveEnemyCount >= MaxEnemies || EnemyClasses.Num() == 0)
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// 랜덤한 Enemy 클래스 선택
	int32 Index = FMath::RandRange(0, EnemyClasses.Num() - 1);
	TSubclassOf<AEnemy> SelectedEnemyClass = EnemyClasses[Index];

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;

	AEnemy* NewEnemy = World->SpawnActor<AEnemy>(SelectedEnemyClass, Location, Rotation, SpawnParams);
	if (NewEnemy)
	{
		ActiveEnemyCount++;
		NewEnemy->OwningSpawner = this;

		AEnemy* DefaultEnemy = Cast<AEnemy>(SelectedEnemyClass->GetDefaultObject());
		if (DefaultEnemy && DefaultEnemy->SpecialAnimMontages.Num() > 0)
		{
			NewEnemy->SpecialAnimMontages = DefaultEnemy->SpecialAnimMontages;
		}

		NewEnemy->InitializeRandomAnimation();
	}

	return NewEnemy;
}

void AEnemySpawner::OnEnemyDestroyed(FVector LastLocation, FRotator LastRotation)
{
	if (ActiveEnemyCount > 0)
	{
		ActiveEnemyCount--;
	}

	SpawnEnemy(LastLocation, LastRotation);

	UE_LOG(LogTemp, Warning, TEXT("Enemy destroyed. Active count: %d"), ActiveEnemyCount);
}

FVector AEnemySpawner::GetRandomSpawnLocation()
{
	FVector SpawnerLocation = GetActorLocation();
	float RandomX = FMath::RandRange(-SpawnAreaExtent.X, SpawnAreaExtent.X);
	float RandomY = FMath::RandRange(-SpawnAreaExtent.Y, SpawnAreaExtent.Y);

	FVector TraceStart = SpawnerLocation + FVector(RandomX, RandomY, 1000.0f);
	FVector TraceEnd = SpawnerLocation + FVector(RandomX, RandomY, -1000.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		return HitResult.Location + FVector(0, 0, 100.0f);
	}

	return SpawnerLocation + FVector(RandomX, RandomY, 0.0f);
}

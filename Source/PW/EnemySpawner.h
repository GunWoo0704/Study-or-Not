// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PW_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 적 생성 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AEnemy* SpawnEnemy(FVector Location, FRotator Rotation);

	// 적이 죽었을 때 호출될 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void OnEnemyDestroyed(FVector LastLocation, FRotator LastRotation);

	// 현재 활성화된 적의 수
	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	int32 ActiveEnemyCount;

	// 생성할 적 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<AEnemy>> EnemyClasses;

	// 최대 적 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxEnemies;

	// 초기 생성할 적의 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 InitialEnemyCount;

	// 적 생성 영역 (박스 영역의 크기)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector SpawnAreaExtent;

private:
	// 랜덤 위치 생성 함수
	FVector GetRandomSpawnLocation();
};
// Fill out your copyright notice in the Description page of Project Settings.

#include "PWGameMode.h"
#include "Kismet/GameplayStatics.h"

APWGameMode::APWGameMode()
{
	// 기본 설정
	PrimaryActorTick.bCanEverTick = false;

	// 점수 초기화
	CurrentScore = 0;
}

void APWGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 점수 초기화
	ResetScore();

	UE_LOG(LogTemp, Warning, TEXT("Game started. Initial score: %d"), CurrentScore);
}

void APWGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Warning, TEXT("Game ended. Final score: %d"), CurrentScore);
}

void APWGameMode::AddScore(int32 ScoreToAdd)
{
	/*if (ScoreToAdd <= 0)
	{
		return;
	}*/

	CurrentScore += ScoreToAdd;
	UE_LOG(LogTemp, Warning, TEXT("Score increased by %d. New score: %d"), ScoreToAdd, CurrentScore);

	// 델리게이트 브로드캐스트
	OnScoreChanged.Broadcast(CurrentScore);
}

void APWGameMode::ResetScore()
{
	CurrentScore = 0;
	UE_LOG(LogTemp, Warning, TEXT("Score reset to 0"));

	// 델리게이트 브로드캐스트
	OnScoreChanged.Broadcast(CurrentScore);
}
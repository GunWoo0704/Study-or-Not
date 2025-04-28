// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PWGameMode.generated.h"

/**
 * 기본 게임 모드 클래스
 */
UCLASS()
class PW_API APWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APWGameMode();

	// 현재 점수
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 CurrentScore;

	// 점수 증가 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 ScoreToAdd);

	// 점수 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void ResetScore();

	// 점수 변경 시 호출되는 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, NewScore);

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChangedSignature OnScoreChanged;

	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 게임 종료 시 호출
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PWGameMode.h"
#include "ScoreWidget.generated.h"

/**
 * 점수에 따라 이미지를 표시하는 위젯 클래스
 */
UCLASS()
class PW_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 초기화 함수
	virtual void NativeConstruct() override;

	// 점수 변경 시 호출되는 함수
	UFUNCTION()
	void OnScoreUpdate(int32 NewScore);

	// 점수에 따라 이미지 업데이트
	UFUNCTION(BlueprintCallable, Category = "UI|Score")
	void UpdateScoreImage(int32 Score);

protected:
	// UI에 표시될 이미지 위젯
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ScoreImage;

	// 점수 등급별 이미지 (F, D, C, B, A, S 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_F;  // 0-9점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_C0;  // 10-19점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_CP;  // 20-29점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_BM;  // 30-39점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_B;  // 40-49점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_BP;  // 50점 이상

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_AM;  // 50점 이상

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_A;  // 50점 이상

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Score")
	UTexture2D* ScoreImage_AP;  // 50점 이상

	// 이 선언이 있는지 확인하세요
	UFUNCTION(BlueprintCallable, Category = "UI|Score")
	void InitialScore(int32 TestScore);

private:
	// 게임모드 레퍼런스
	APWGameMode* GameMode;
};
// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreWidget.h"
#include "Kismet/GameplayStatics.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 게임모드 레퍼런스 가져오기
	GameMode = Cast<APWGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		// 점수 변경 이벤트에 함수 바인딩
		// 오류 수정: UScoreUpdate -> OnScoreUpdate
		GameMode->OnScoreChanged.AddDynamic(this, &UScoreWidget::OnScoreUpdate);

		// 현재 점수로 초기 이미지 설정
		UpdateScoreImage(GameMode->CurrentScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ScoreWidget: GameMode is null!"));
	}
}

void UScoreWidget::OnScoreUpdate(int32 NewScore)
{
	// 점수가 변경될 때 이미지 업데이트
	UpdateScoreImage(NewScore);
}

void UScoreWidget::UpdateScoreImage(int32 Score)
{
	if (!ScoreImage)
	{
		UE_LOG(LogTemp, Error, TEXT("ScoreWidget: ScoreImage is null!"));
		return;
	}

	// 점수 구간에 따라 적절한 이미지 선택
	UTexture2D* SelectedTexture = nullptr;

	if (Score < 100) // 100 미만일 때 F 이미지 표시
	{
		SelectedTexture = ScoreImage_F;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to F (Score: %d)"), Score);
	}
	else if (Score >= 110 && Score <= 150)
	{
		SelectedTexture = ScoreImage_C0;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to C0 (Score: %d)"), Score);
	}
	else if (Score >= 160 && Score <= 200)
	{
		SelectedTexture = ScoreImage_CP;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to CP (Score: %d)"), Score);
	}
	else if (Score >= 210 && Score <= 250)
	{
		SelectedTexture = ScoreImage_BM;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to BM (Score: %d)"), Score);
	}
	else if (Score >= 260 && Score <= 300)
	{
		SelectedTexture = ScoreImage_B;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to B (Score: %d)"), Score);
	}
	else if (Score >= 310 && Score <= 350)
	{
		SelectedTexture = ScoreImage_BP;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to BP (Score: %d)"), Score);
	}
	else if (Score >= 360 && Score <= 400)
	{
		SelectedTexture = ScoreImage_AM;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to AM (Score: %d)"), Score);
	}
	else if (Score >= 410 && Score < 450)
	{
		SelectedTexture = ScoreImage_A;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to A (Score: %d)"), Score);
	}
	else if (Score >= 450)
	{
		SelectedTexture = ScoreImage_AP;
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Setting image to AP (Score: %d)"), Score);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ScoreWidget: Score out of range (Score: %d)"), Score);
	}
	// 선택된 텍스처가 유효한지 확인하고 적용
	if (SelectedTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(SelectedTexture);
		ScoreImage->SetBrush(Brush);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ScoreWidget: Selected texture is null for score %d"), Score);
	}
}

void UScoreWidget::InitialScore(int32 TestScore)
{
	UE_LOG(LogTemp, Warning, TEXT("ScoreWidget: Testing with score %d"), TestScore);
	UpdateScoreImage(TestScore);
}
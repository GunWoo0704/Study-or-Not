// Fill out your copyright notice in the Description page of Project Settings.

#include "PWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "PWGameMode.h"

void APWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// 위젯 클래스가 설정되어 있는지 확인
	if (ScoreWidgetClass)
	{
		// 위젯 생성
		ScoreWidget = CreateWidget<UUserWidget>(this, ScoreWidgetClass);
		if (ScoreWidget)
		{
			// 화면에 위젯 추가
			ScoreWidget->AddToViewport();

			// 델리게이트 바인딩
			if (APWGameMode* GameMode = Cast<APWGameMode>(GetWorld()->GetAuthGameMode()))
			{
				GameMode->OnScoreChanged.AddDynamic(this, &APWPlayerController::OnScoreUpdated);

				// 초기 점수 설정
				OnScoreUpdated(GameMode->CurrentScore);
			}
		}
	}
}

void APWPlayerController::OnScoreUpdated(int32 NewScore)
{
	if (ScoreWidget)
	{
		// UpdateScore 함수 호출
		UFunction* UpdateScoreFunc = ScoreWidget->FindFunction(FName("UpdateScore"));
		if (UpdateScoreFunc)
		{
			struct
			{
				int32 NewScore;
			} Params;

			Params.NewScore = NewScore;
			ScoreWidget->ProcessEvent(UpdateScoreFunc, &Params);
		}
	}
}
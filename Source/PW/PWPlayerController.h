// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PWPlayerController.generated.h"

/**
 *
 */
UCLASS()
class PW_API APWPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;

	// Begin Actor interface
	virtual void BeginPlay() override;
	// End Actor interface

	// 점수 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> ScoreWidgetClass;

	// 점수 위젯 인스턴스
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	class UUserWidget* ScoreWidget;

	// 점수 UI 업데이트
	UFUNCTION()
	void OnScoreUpdated(int32 NewScore);
};
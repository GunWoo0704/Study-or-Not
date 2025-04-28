// UMySpeechBubbleWidget.h
#pragma once

#include "CoreMinimal.h"                       // ← CoreMinimal 먼저
#include "Blueprint/UserWidget.h"              // ← 그다음 필요한 UE 헤더
#include "UMySpeechBubbleWidget.generated.h"   // ← 반드시 마지막 include

UCLASS()
class PW_API UMySpeechBubbleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 블루프린트에서 구현: 말풍선 텍스트 세팅
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void SetDialogueText(const FText& InText);
};

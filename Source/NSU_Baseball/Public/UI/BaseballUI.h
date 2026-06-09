#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballUI.generated.h"

class UTextBlock;
class UButton;
class UEditableText;

UCLASS()
class NSU_BASEBALL_API UBaseballUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// 서버에서 받은 결과값 설정
	void SetResultText(const FString& ResultMsg);
	
	void UpdateTurnDisplay(APlayerState* TurnPlayer);
	void UpdateTimerDisplay(int32 NewTime);
	void UpdateAttemptDisplay(int32 UsedCount);
	
	void ShowWarningMessage(const FString& WarningMsg);
	
protected:
	virtual void NativeConstruct() override;
	
	// 입력 박스
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> InputTextBox;
	
	// 제출 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SubmitButton;
	
	// 결과 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextLastResult;
	
	// 턴 정보
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextTurnInfo;
	
	// 남은 시간 정보
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextTimer;
	
	// 시도 횟수
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextMyAttempts;
	
	// 경고 문구
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextWarning;
	
	// 경고 문구 페이드 아웃 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOutWarningAnim;
	
	UFUNCTION()
	void OnSubmitButtonClicked();
	
	UFUNCTION()
	void OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};

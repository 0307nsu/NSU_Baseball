#include "UI/BaseballUI.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Controller/BaseballPlayerController.h"
#include "GameFramework/PlayerState.h"

void UBaseballUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SubmitButton)
	{
		SubmitButton->OnClicked.AddDynamic(this, &UBaseballUI::OnSubmitButtonClicked);
	}
}

void UBaseballUI::OnSubmitButtonClicked()
{
	if (!InputTextBox) return;
	
	FString InputStr = InputTextBox->GetText().ToString();
	
	if (!InputStr.IsEmpty())
	{
		if (ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(GetOwningPlayer()))
		{
			PC->SubmitInput(InputStr);
		}
		
		InputTextBox->SetText(FText::GetEmpty());
	}
}

void UBaseballUI::SetResultText(const FString& ResultMsg)
{
	if (TextLastResult)
	{
		TextLastResult->SetText(FText::FromString(ResultMsg));
	}
}

void UBaseballUI::UpdateTurnDisplay(APlayerState* TurnPlayer)
{
	if (TextTurnInfo && TurnPlayer)
	{
		FString TurnMsg = FString::Printf(TEXT("%s의 턴입니다."), *TurnPlayer->GetPlayerName());
		TextTurnInfo->SetText(FText::FromString(TurnMsg));
	}
}

void UBaseballUI::UpdateTimerDisplay(int32 NewTime)
{
	if (TextTimer)
	{
		TextTimer->SetText(FText::AsNumber(NewTime));
	}
}

void UBaseballUI::UpdateAttemptDisplay(int32 UsedCount)
{
	if (TextMyAttempts)
	{
		FString AttemptMsg = FString::Printf(TEXT("내 시도 횟수 : %d / 3"), UsedCount);
		TextMyAttempts->SetText(FText::FromString(AttemptMsg));
	}
}

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
	
	if (InputTextBox)
	{
		InputTextBox->OnTextCommitted.AddDynamic(this, &UBaseballUI::OnInputTextCommitted);
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

void UBaseballUI::OnInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnSubmitButtonClicked();
	}
}

void UBaseballUI::SetResultText(const FString& ResultMsg)
{
	if (TextLastResult)
	{
		FString CurrentText = TextLastResult->GetText().ToString();
		
		if (CurrentText.IsEmpty())
		{
			TextLastResult->SetText(FText::FromString(ResultMsg));
		}

		else
		{
			TextLastResult->SetText(FText::FromString(CurrentText + TEXT("\n") + ResultMsg));
		}
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

void UBaseballUI::ShowWarningMessage(const FString& WarningMsg)
{
	if (TextWarning)
	{
		TextWarning->SetText(FText::FromString(WarningMsg));
	}
	
	if (FadeOutWarningAnim)
	{
		PlayAnimation(FadeOutWarningAnim);
	}
}

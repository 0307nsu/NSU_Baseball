#include "UI/BaseballUI.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Controller/BaseballPlayerController.h"

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
	
}

void UBaseballUI::UpdateTimerDisplay(int32 NewTime)
{
	
}

void UBaseballUI::UpdateAttemptDisplay(int32 UsedCount)
{
	
}

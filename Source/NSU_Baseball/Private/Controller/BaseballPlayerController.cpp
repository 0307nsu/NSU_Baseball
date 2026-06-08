#include "Controller/BaseballPlayerController.h"
#include "GameMode/BaseballGameModeBase.h"
#include "UI/BaseballUI.h"

void ABaseballPlayerController::SubmitInput(const FString& InputStr)
{
	ServerSubmitInput(InputStr);
}

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController())
	{
		if (BaseballUIClass)
    	{
    		BaseballUI = CreateWidget<UBaseballUI>(this, BaseballUIClass);
    		if (BaseballUI)
    		{
    			BaseballUI->AddToViewport();
    			
    			FInputModeUIOnly InputMode;
    			InputMode.SetWidgetToFocus(BaseballUI->TakeWidget());
    			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    			
    			SetInputMode(InputMode);
    			bShowMouseCursor = true;
    		}
    	}
	}
}

void ABaseballPlayerController::ServerSubmitInput_Implementation(const FString& InputStr)
{
	if (HasAuthority())
	{
		if (ABaseballGameModeBase* GM = Cast<ABaseballGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GM->ProcessPlayerInput(this, InputStr);
		}
	}
}

void ABaseballPlayerController::ClientReceiveResult_Implementation(const FString& ResultMsg)
{
	if (BaseballUI)
	{
		BaseballUI->SetResultText(ResultMsg);
	}
}

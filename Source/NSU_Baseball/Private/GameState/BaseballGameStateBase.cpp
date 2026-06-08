#include "GameState/BaseballGameStateBase.h"
#include "Controller/BaseballPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UI/BaseballUI.h"

ABaseballGameStateBase::ABaseballGameStateBase()
{
	CurrentTurnPlayer = nullptr;
	RemainTime = 30;
}

void ABaseballGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ABaseballGameStateBase, CurrentTurnPlayer, COND_None);
	DOREPLIFETIME_CONDITION(ABaseballGameStateBase, RemainTime, COND_None);
}

void ABaseballGameStateBase::OnRep_CurrentTurnPlayer()
{
	if (APlayerController* LocalPC = GetWorld()->GetFirstPlayerController())
	{
		if (ABaseballPlayerController* BaseballPC = Cast<ABaseballPlayerController>(LocalPC))
		{
			if (BaseballPC->BaseballUI)
			{
				BaseballPC->BaseballUI->UpdateTurnDisplay(CurrentTurnPlayer.Get());
			}
		}
	}
}

void ABaseballGameStateBase::OnRep_RemainTime()
{
	if (APlayerController* LocalPC = GetWorld()->GetFirstPlayerController())
	{
		if (ABaseballPlayerController* BaseballPC = Cast<ABaseballPlayerController>(LocalPC))
		{
			if (BaseballPC->BaseballUI)
			{
				BaseballPC->BaseballUI->UpdateTimerDisplay(RemainTime);
			}
		}
	}
}

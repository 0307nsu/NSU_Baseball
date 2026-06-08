#include "PlayerState/BaseballPlayerState.h"
#include "Controller/BaseballPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UI/BaseballUI.h"

ABaseballPlayerState::ABaseballPlayerState()
{
	UsedAttemptCount = 0;
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseballPlayerState, UsedAttemptCount);
}

void ABaseballPlayerState::IncrementAttempt()
{
	if (HasAuthority())
	{
		++UsedAttemptCount;
	}
}

void ABaseballPlayerState::OnRep_UsedAttemptCount()
{
	APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
	if (LocalPC->PlayerState == this)
	{
		ABaseballPlayerController* BaseballPC = Cast<ABaseballPlayerController>(LocalPC);
		if (BaseballPC->BaseballUI)
		{
			BaseballPC->BaseballUI->UpdateAttemptDisplay(UsedAttemptCount);
		}
	}
}

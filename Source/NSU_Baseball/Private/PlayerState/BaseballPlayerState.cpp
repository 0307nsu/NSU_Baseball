#include "PlayerState/BaseballPlayerState.h"
#include "Net/UnrealNetwork.h"

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

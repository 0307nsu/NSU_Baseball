#include "GameMode/BaseballGameModeBase.h"

#include "Controller/BaseballPlayerController.h"
#include "GameState/BaseballGameStateBase.h"
#include "PlayerState/BaseballPlayerState.h"

void ABaseballGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	PlayerTargets.Add(NewPlayer, GenerateTargetNumber());
}

void ABaseballGameModeBase::ProcessPlayerInput(APlayerController* PC, const FString& InputStr)
{
	if (!PC || !PlayerTargets.Contains(PC)) return;
	
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS) return;
	
	if (PC->GetPlayerState<APlayerState>() != GS->CurrentTurnPlayer)
	{
		return;
	}
	
	if (!IsValidInput(InputStr)) return;
	
	FString TargetNumber = PlayerTargets[PC];
	int32 Strikes = 0;
	int32 Balls = 0;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (InputStr[i] == TargetNumber[i])
		{
			++Strikes;
		}
		
		else if (TargetNumber.Contains(FString::Printf(TEXT("%c"), InputStr[i])))
		{
			++Balls;
		}
	}
	
	FString ResultMessage;
	if (Strikes == 0 && Balls == 0)
	{
		ResultMessage = TEXT("OUT");
	}

	else
	{
		ResultMessage = FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
	}
	
	ABaseballPlayerController* BaseballPC = Cast<ABaseballPlayerController>(PC);
	if (BaseballPC)
	{
		BaseballPC->ClientReceiveResult(ResultMessage);
	}
	
	if (ABaseballPlayerState* PS = PC->GetPlayerState<ABaseballPlayerState>())
	{
		PS->IncrementAttempt();
	}
	
	if (Strikes == 3)
	{
		EndGame(PC);
		return;
	}
	
	AdvanceTurn();
}

void ABaseballGameModeBase::StartGame()
{
	CurrentTurnIndex = 0;
	StartTurn();
}

void ABaseballGameModeBase::StartTurn()
{
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS || GS->PlayerArray.Num() == 0) return;
	
	GS->CurrentTurnPlayer = GS->PlayerArray[CurrentTurnIndex];
	GS->RemainTime = 30;
		
	ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(GS->CurrentTurnPlayer);
	if (PS->UsedAttemptCount >= 3)
	{
		AdvanceTurn();
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &ABaseballGameModeBase::UpdateTimer, 1.f, true);
}

void ABaseballGameModeBase::UpdateTimer()
{
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS) return;
	
	GS->RemainTime--;
	
	if (GS->RemainTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
		
		if (GS->CurrentTurnPlayer)
		{
			ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(GS->CurrentTurnPlayer);
			if (PS)
			{
				PS->IncrementAttempt();
			}
		}
		
		AdvanceTurn();
	}
}

void ABaseballGameModeBase::AdvanceTurn()
{
	GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS) return;
	
	if (CheckDrawCondition())
	{
		EndGame(nullptr);
		return;
	}
	
	CurrentTurnIndex = (CurrentTurnIndex + 1) % GS->PlayerArray.Num();
	
	StartTurn();
}

void ABaseballGameModeBase::EndGame(APlayerController* Winner)
{
	GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	
	if (Winner)
	{
		UE_LOG(LogTemp, Warning, TEXT("게임 종료. 승리자: %s"), *Winner->GetName());
		//TODO: 승리 처리
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("게임 종료. 무승부."));
	}
}

bool ABaseballGameModeBase::CheckDrawCondition()
{
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS) return false;
	
	for (APlayerState* PlayerState : GS->PlayerArray)
	{
		if (ABaseballPlayerState* PS = Cast<ABaseballPlayerState>(PlayerState))
		{
			if (PS->UsedAttemptCount < 3)
			{
				return false;
			}
		}
	}
	
	return true;
}

FString ABaseballGameModeBase::GenerateTargetNumber()
{
	TArray<int32> Digits = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	FString Target = TEXT("");
	
	for (int32 i = 0; i < 3; ++i)
	{
		int32 RandIdx = FMath::RandRange(0, Digits.Num() - 1);
		Target += FString::FromInt(Digits[RandIdx]);
		Digits.RemoveAt(RandIdx);
	}
	
	return Target;
}

bool ABaseballGameModeBase::IsValidInput(const FString& InputStr)
{
	if (InputStr.Len() != 3) return false;
	
	if (!InputStr.IsNumeric()) return false;
	
	return true;
}

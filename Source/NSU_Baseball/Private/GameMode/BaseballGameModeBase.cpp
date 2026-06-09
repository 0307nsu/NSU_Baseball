#include "GameMode/BaseballGameModeBase.h"
#include "Controller/BaseballPlayerController.h"
#include "GameState/BaseballGameStateBase.h"
#include "PlayerState/BaseballPlayerState.h"

void ABaseballGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	PlayerTargets.Add(NewPlayer, GenerateTargetNumber());
	
	if (AGameStateBase* GS = GetGameState<AGameStateBase>())
	{
		if (GS->PlayerArray.Num() == 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("2명 접속 완료. 게임 시작."));
			StartGame();
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("현재 접속 인원: %d명. 대기 중..."), GS->PlayerArray.Num());
		}
	}
}

void ABaseballGameModeBase::ProcessPlayerInput(APlayerController* PC, const FString& InputStr)
{
	if (bIsGameOver) return;
	
	if (!PC || !PlayerTargets.Contains(PC)) return;
	
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS) return;
	
	if (PC->GetPlayerState<APlayerState>() != GS->CurrentTurnPlayer)
	{
		if (ABaseballPlayerController* BaseballPC = Cast<ABaseballPlayerController>(PC))
		{
			BaseballPC->ClientReceiveWarning(TEXT("상대 턴입니다. 기다려주세요."));
		}
		return;
	}
	
	if (!IsValidInput(InputStr))
	{
		ABaseballPlayerController* BaseballPC = Cast<ABaseballPlayerController>(PC);
		if (BaseballPC)
		{
			BaseballPC->ClientReceiveWarning(TEXT("잘못된 입력입니다. 1~9 중 숫자 3개를 입력하세요."));
		}
		return;
	}
	
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
		ResultMessage = FString::Printf(TEXT("%s -> OUT"), *InputStr);
	}

	else
	{
		ResultMessage = FString::Printf(TEXT("%s -> %dS%dB"), *InputStr, Strikes, Balls);
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
	bIsGameOver = false;
	CurrentTurnIndex = 0;
	StartTurn();
}

void ABaseballGameModeBase::StartTurn()
{
	ABaseballGameStateBase* GS = GetGameState<ABaseballGameStateBase>();
	if (!GS || GS->PlayerArray.Num() == 0) return;
	
	GS->CurrentTurnPlayer = GS->PlayerArray[CurrentTurnIndex];
	GS->RemainTime = 30;
	GS->OnRep_CurrentTurnPlayer();
	GS->OnRep_RemainTime();
		
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
	bIsGameOver = true;
	FString EndMsg;
	
	if (Winner)
	{
		EndMsg = FString::Printf(TEXT("게임 종료! 승리자: %s"), *Winner->PlayerState->GetPlayerName());
	}

	else
	{
		EndMsg = TEXT("게임 종료! 무승부입니다.");
	}
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(It->Get());
		if (PC)
		{
			PC->ClientReceiveResult(EndMsg);
		}
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
	
	if (InputStr.Contains(TEXT("0"))) return false;
	
	TSet<TCHAR> UniqueChars;
	for (int32 i = 0; i < 3; ++i)
	{
		UniqueChars.Add(InputStr[i]);
	}
	
	if (UniqueChars.Num() != 3) return false;
	
	return true;
}

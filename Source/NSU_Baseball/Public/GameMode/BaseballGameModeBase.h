#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseballGameModeBase.generated.h"

UCLASS()
class NSU_BASEBALL_API ABaseballGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	// 플레이어 입력 처리
	void ProcessPlayerInput(APlayerController* PC, const FString& InputStr);
	
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Baseball")
	void StartGame();
	
private:
	// 플레이어별 정답
	TMap<TObjectPtr<APlayerController>, FString> PlayerTargets;
	
	int32 CurrentTurnIndex = 0;
	FTimerHandle TurnTimerHandle;
	
	// 숫자 랜덤 생성
	FString	GenerateTargetNumber();
	
	bool bIsGameOver = false;
	
	// 플레이어 입력 검증
	bool IsValidInput(const FString& InputStr);
	
	void StartTurn();
	void UpdateTimer();
	void AdvanceTurn();
	void EndGame(APlayerController* Winner);
	bool CheckDrawCondition();
};

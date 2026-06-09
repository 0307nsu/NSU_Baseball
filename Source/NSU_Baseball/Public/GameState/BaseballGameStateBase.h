#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseballGameStateBase.generated.h"

UCLASS()
class NSU_BASEBALL_API ABaseballGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABaseballGameStateBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 현재 턴인 플레이어의 PlayerState
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnPlayer, BlueprintReadOnly, Category = "Baseball")
	TObjectPtr<APlayerState> CurrentTurnPlayer;
	
	// 현재 턴의 남은 시간
	UPROPERTY(ReplicatedUsing = OnRep_RemainTime, BlueprintReadOnly, Category = "Baseball")
	int32 RemainTime;
	
	UFUNCTION()
	void OnRep_CurrentTurnPlayer();
	
	UFUNCTION()
	void OnRep_RemainTime();
};

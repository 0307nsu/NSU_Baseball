#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

UCLASS()
class NSU_BASEBALL_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABaseballPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 사용한 기회
	UPROPERTY(ReplicatedUsing = OnRep_UsedAttemptCount, BlueprintReadOnly, Category = "Baseball")
	int32 UsedAttemptCount;
	
	// 기회 소모
	void IncrementAttempt();
	
	UFUNCTION()
	void OnRep_UsedAttemptCount();
};

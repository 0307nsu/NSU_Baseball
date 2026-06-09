#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

class UBaseballUI;

UCLASS()
class NSU_BASEBALL_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 플레이어의 입력 제출
	UFUNCTION(BlueprintCallable, Category = "Baseball")
	void SubmitInput(const FString& InputStr);
	
	// 서버로 입력 제출
	UFUNCTION(Server, Reliable)
	void ServerSubmitInput(const FString& InputStr);
	
	// 서버에서 결과를 받아와 클라이언트 UI에 출력
	UFUNCTION(Client, Reliable)
	void ClientReceiveResult(const FString& ResultMsg);
	
	// 경고 메시지 출력
	UFUNCTION(Client, Reliable)
	void ClientReceiveWarning(const FString& WarningMsg);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBaseballUI> BaseballUIClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UBaseballUI> BaseballUI;
	
protected:
	virtual void BeginPlay() override;
};

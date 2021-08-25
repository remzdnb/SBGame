#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SB_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateUpdatedDelegate);

UCLASS()
class ASB_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ASB_GameState();
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	//

	/*UFUNCTION()
	void InitGame();

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void StopGame();

	UFUNCTION()
	void EndGame();

	UFUNCTION()
	void ResetGame();*/

	//

	void BroadcastGameStateUpdated();

	//

	FGameStateUpdatedDelegate OnGameStateUpdated;

	//

	UPROPERTY()
	ESB_GameType GameType;
	
	UPROPERTY()
	ESB_GamePhase GamePhase;

	UPROPERTY()
	TArray<FSB_TeamData> TeamsData;

private:

	FTimerHandle GamePhaseTimer;
};

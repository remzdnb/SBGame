#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SB_GameState.generated.h"

class USB_GameInstance;
class ASB_PlayerStart;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateUpdatedDelegate);

UCLASS()
class ASB_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ASB_GameState();

	virtual void PostInitializeComponents() override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	//

	UFUNCTION()
	void AddPlayerToTeam(AController* PlayerToAdd, uint8 TeamID);

	UFUNCTION()
	void RemovePlayerFromTeam(AController* const PlayerToRemove);

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

	UFUNCTION()
	void InitTeams();

	//

	USB_GameInstance* GInstance;
	TArray<ASB_PlayerStart*> PlayerStarts;
	FTimerHandle GamePhaseTimer;
};

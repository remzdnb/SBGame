#pragma once

#include "SB_Types.h"
#include "Battle/SB_BattleGameMode.h"
//
#include "CoreMinimal.h"
#include "SB_MatchGameMode.generated.h"

UCLASS()
class ASB_MatchGameMode : public ASB_BattleGameMode
{
	GENERATED_BODY()

public:

	ASB_MatchGameMode();

	virtual void PostInitializeComponents() override;

private:
	
	UFUNCTION(Exec)
	void SetupGame();

	UFUNCTION(Exec)
	void StartGame();

	UFUNCTION(Exec)
	void StopGame();

	UFUNCTION(Exec)
	void ResetGame();
};

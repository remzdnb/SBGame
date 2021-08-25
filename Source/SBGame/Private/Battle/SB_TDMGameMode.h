#pragma once

#include "SB_Types.h"
#include "Battle/SB_BattleGameMode.h"
//
#include "CoreMinimal.h"
#include "SB_TDMGameMode.generated.h"

UCLASS()
class ASB_TDMGameMode : public ASB_BattleGameMode
{
	GENERATED_BODY()

public:

	ASB_TDMGameMode();

private:
	
	UFUNCTION(Exec)
	void SetupGame();

	UFUNCTION(Exec)
	void StartGame();

	UFUNCTION(Exec)
	void StopGame();

	UFUNCTION(Exec)
	void EndGame();

	UFUNCTION(Exec)
	void ResetGame();
};

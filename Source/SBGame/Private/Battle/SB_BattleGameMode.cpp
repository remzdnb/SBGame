#include "Battle/SB_BattleGameMode.h"
#include "Battle/SB_BattlePlayerController.h"

ASB_BattleGameMode::ASB_BattleGameMode()
{
	PlayerControllerClass = ASB_BattlePlayerController::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}
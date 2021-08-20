#include "Battle/SB_TDMGameMode.h"
#include "Battle/SB_BattlePlayerController.h"

ASB_TDMGameMode::ASB_TDMGameMode()
{
	PlayerControllerClass = ASB_BattlePlayerController::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}
#include "Battle/SB_TDMGameMode.h"
#include "Battle/SB_BattlePlayerController.h"
#include "Vehicle/SB_Vehicle.h"
#include "SB_GameState.h"
#include "SB_PlayerStart.h"

ASB_TDMGameMode::ASB_TDMGameMode()
{
	PlayerControllerClass = ASB_BattlePlayerController::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}

void ASB_TDMGameMode::SetupGame()
{
	for (const auto& TeamData : GState->TeamsData)
	{
		uint8 Index = 0;
		for (const auto& Controller : TeamData.PlayerList)
		{
			ISB_ControllerInterface* const ControllerInterface = Cast<ISB_ControllerInterface>(Controller);
			if (ControllerInterface)
			{
				ASB_Vehicle* const NewVehicle = ControllerInterface->SpawnAndPossessVehicle(
					TeamData.PlayerStartList[Index]->GetTransform()
				);
				
				if (NewVehicle)
				{
					RegisterVehicle(NewVehicle);
					//NewVehicle->OnVehicleDestroyed.AddDynamic(this, &ASB_FSGameMode::UnregisterVehicle);
				}
			}

			Index++;
		}
	}
}

void ASB_TDMGameMode::StartGame()
{
	/*TArray<ASB_PlayerStart*> AvailablePlayerStarts;
	TArray

	for (auto& )*/
}

void ASB_TDMGameMode::StopGame()
{
}

void ASB_TDMGameMode::EndGame()
{
}

void ASB_TDMGameMode::ResetGame()
{
}

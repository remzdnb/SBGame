#include "Battle/SB_MatchGameMode.h"
#include "Battle/SB_BattlePlayerController.h"
#include "Vehicle/SB_Vehicle.h"
#include "SB_GameState.h"
#include "SB_PlayerStart.h"

ASB_MatchGameMode::ASB_MatchGameMode()
{
	PlayerControllerClass = ASB_BattlePlayerController::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}

void ASB_MatchGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GState->GamePhase = ESB_GamePhase::WaitingForPlayers;
}

void ASB_MatchGameMode::SetupGame()
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

	GState->GamePhase = ESB_GamePhase::Ready;
}

void ASB_MatchGameMode::StartGame()
{
	GState->GamePhase = ESB_GamePhase::Playing;
}

void ASB_MatchGameMode::StopGame()
{
	GState->GamePhase = ESB_GamePhase::Stopping;
}

void ASB_MatchGameMode::ResetGame()
{
	GState->GamePhase = ESB_GamePhase::WaitingForPlayers;
}

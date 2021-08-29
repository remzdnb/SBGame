#include "SB_GameState.h"
#include "SB_PlayerState.h"
#include "SB_GameInstance.h"
#include "SB_PlayerStart.h"
#include "SB_GameSettings.h"
//
#include "TimerManager.h"
#include "EngineUtils.h"

ASB_GameState::ASB_GameState()
{
	GameType = ESB_GameType::Battle;
	GamePhase = ESB_GamePhase::Ready;
}

void ASB_GameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (GetWorld() == nullptr || GetWorld()->IsGameWorld() == false)
		return;
	
	GInstance = GetGameInstance<USB_GameInstance>();

	for (TActorIterator<ASB_PlayerStart> PlayerStart(GetWorld()); PlayerStart; ++PlayerStart)
	{
		PlayerStarts.Add(*PlayerStart);
	}

	InitTeams();
}

void ASB_GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	OnGameStateUpdated.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("ASB_GameState - PlayerState added."));
}

void ASB_GameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	OnGameStateUpdated.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("ASB_GameState - PlayerState removed."));
}

void ASB_GameState::AddPlayerToTeam(AController* PlayerToAdd, uint8 TeamID)
{
	if (TeamsData.IsValidIndex(TeamID) == false)
		return;

	ASB_PlayerState* const PlayerState = PlayerToAdd->GetPlayerState<ASB_PlayerState>();

	if (PlayerState->GetTeamID() == TeamID)
		return;
	
	RemovePlayerFromTeam(PlayerToAdd);
	TeamsData[TeamID].PlayerList.Add(PlayerToAdd);
	PlayerState->SetTeamID(TeamID);
	
	OnGameStateUpdated.Broadcast();
}

void ASB_GameState::RemovePlayerFromTeam(AController* const PlayerToRemove)
{
	const ASB_PlayerState* const PlayerState = PlayerToRemove->GetPlayerState<ASB_PlayerState>();
	TeamsData[PlayerState->GetTeamID()].PlayerList.Remove(PlayerToRemove);
}

void ASB_GameState::InitTeams()
{
	TeamsData.SetNum(GInstance->GameSettings->MaxTeams);
	TeamsData[0].MaxPlayers = GInstance->GameSettings->MaxSpectators;
	
	for (const auto& PlayerStart : PlayerStarts)
	{
		if (TeamsData.IsValidIndex(PlayerStart->GetTeamID()))
		{
			if (PlayerStart->GetTeamID() != 0)
			{
				TeamsData[PlayerStart->GetTeamID()].MaxPlayers++;
			}

			TeamsData[PlayerStart->GetTeamID()].PlayerStartList.Add(PlayerStart);
		}
	}

	OnGameStateUpdated.Broadcast();
}

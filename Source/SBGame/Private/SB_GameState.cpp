#include "SB_GameState.h"
//
#include "TimerManager.h"

ASB_GameState::ASB_GameState()
{
	GameType = ESB_GameType::Battle;
	GamePhase = ESB_GamePhase::Ready;
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
/*
void ASB_GameState::InitGame()
{
	if (GamePhase == ESB_GamePhase::Ready)
	{
		GetWorld()->GetTimerManager().SetTimer(GamePhaseTimer, this, &ASB_GameState::StartGame, 5.0f, true, 0.0f);
		
		GamePhase = ESB_GamePhase::Starting;
	}
}

void ASB_GameState::StartGame()
{
	if (GamePhase == ESB_GamePhase::Starting)
	{
		GamePhase = ESB_GamePhase::Playing;
	}
}

void ASB_GameState::StopGame()
{
}

void ASB_GameState::EndGame()
{
}

void ASB_GameState::ResetGame()
{
}*/

void ASB_GameState::BroadcastGameStateUpdated()
{
	OnGameStateUpdated.Broadcast();
}

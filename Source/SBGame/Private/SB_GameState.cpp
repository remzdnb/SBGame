#include "SB_GameState.h"

void ASB_GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	GameStateUpdatedEvent.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("ASB_GameState - PlayerState added."));
}

void ASB_GameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	GameStateUpdatedEvent.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("ASB_GameState - PlayerState removed."));
}

void ASB_GameState::BroadcastGameStateUpdated()
{
	GameStateUpdatedEvent.Broadcast(true);
}

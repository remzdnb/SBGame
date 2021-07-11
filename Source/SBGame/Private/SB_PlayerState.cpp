#include "SB_PlayerState.h"
#include "SB_GameState.h"
//
#include "Net/UnrealNetwork.h"

ASB_PlayerState::ASB_PlayerState() :
	TeamID(0),
	KillCount(0),
	DeathCount(0)
{
}

void ASB_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GState = Cast<ASB_GameState>(GetWorld()->GetGameState());
}

void ASB_PlayerState::SetName(bool bIsPlayer)
{
	const FString NameString = bIsPlayer ? "Player_" + FMath::RandRange(0, 9999) : "Bot_" + FMath::RandRange(0, 9999);
	DisplayName = *(NameString);
	GState->BroadcastGameStateUpdated();
}

void ASB_PlayerState::SetTeamID(uint8 NewTeamID)
{
	TeamID = NewTeamID;

	GState->BroadcastGameStateUpdated();
}

void ASB_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_PlayerState, TeamID);
	DOREPLIFETIME(ASB_PlayerState, KillCount);
	DOREPLIFETIME(ASB_PlayerState, DeathCount);
}


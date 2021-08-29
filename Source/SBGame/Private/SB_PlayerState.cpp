#include "SB_PlayerState.h"
#include "SB_PlayerSaveGame.h"
#include "SB_GameState.h"
//
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASB_PlayerState::ASB_PlayerState() :
	TeamID(1),
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

	PSaveGame = Cast<USB_PlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveGame", 0));
	if (PSaveGame)
	{
		
	}
	else
	{
		PSaveGame = Cast<USB_PlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(USB_PlayerSaveGame::StaticClass()));
		if (PSaveGame)
		{
			UGameplayStatics::SaveGameToSlot(PSaveGame, "PlayerSaveGame", 0);
		}
	}
}

void ASB_PlayerState::SetName(bool bIsPlayer, const FString& NewPlayerName)
{
	DisplayName = bIsPlayer ? NewPlayerName : "Bot_" + FString::FromInt(FMath::RandRange(0, 9999));
	//GState->BroadcastGameStateUpdated();
}

void ASB_PlayerState::SetTeamID(uint8 NewTeamID)
{
	TeamID = NewTeamID;

	//GState->BroadcastGameStateUpdated();
}

void ASB_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASB_PlayerState, TeamID);
	DOREPLIFETIME(ASB_PlayerState, KillCount);
	DOREPLIFETIME(ASB_PlayerState, DeathCount);
}


#include "SB_GameInstance.h"
#include "SB_DataManager.h"
#include "SB_PlayerSaveGame.h"
//
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void USB_GameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

void USB_GameInstance::OnStart()
{
	Super::OnStart();
}

void USB_GameInstance::Init()
{
	Super::Init();

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	CreateNewSaveGame();
	//Debug();
}

void USB_GameInstance::CreateNewSaveGame()
{
	SaveGame = Cast<USB_PlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSaveGame", 0));
	if (SaveGame)
	{
		
	}
	else
	{
		SaveGame = Cast<USB_PlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(USB_PlayerSaveGame::StaticClass()));
		if (SaveGame)
		{
			UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);
		}
	}
}

void USB_GameInstance::ApplySaveGame()
{
	if (SaveGame)
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);
	}

	Debug();
}

void USB_GameInstance::SetNewShipConfig(const TArray<FName>& NewShipConfig)
{
	if (SaveGame)
	{
		SaveGame->ModulesRowNames = NewShipConfig;
		UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);

		UE_LOG(LogTemp, Warning, TEXT("USB_GameInstance::SetNewShipConfig : Game Saved"));
	}
}

void USB_GameInstance::Debug()
{
	if (SaveGame)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SaveGame // PlayerName : " + SaveGame->PlayerName);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SaveGame // MapName : " + SaveGame->MapName);
	}
}

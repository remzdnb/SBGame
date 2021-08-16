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
		//DataManager = *NewDataManager;
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

void USB_GameInstance::SavePlayerName(const FString& NewPlayerName)
{
	if (SaveGame)
	{
		SaveGame->PlayerName = NewPlayerName;
		UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);
	}
}

void USB_GameInstance::SaveSelectedShip(const FName& NewSelectedShipName)
{
	if (SaveGame)
	{
		SaveGame->ShipDataRowName = NewSelectedShipName;
		UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);
	}
}

void USB_GameInstance::SaveShipConfig(const TArray<FName>& NewShipConfig)
{
	if (SaveGame)
	{
		SaveGame->ShipConfig = NewShipConfig;
		UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);
	}
}

void USB_GameInstance::ResetShipConfig()
{
	if (SaveGame)
	{
		SaveGame->ShipConfig.Empty();
		UGameplayStatics::SaveGameToSlot(SaveGame, "PlayerSaveGame", 0);
	}
}

const FSB_ShipData* const USB_GameInstance::GetShipDataFromRow(const FName& RowName) const
{
	if (ShipDT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetShipDataFromRow : Missing ShipDT."));
		return nullptr;
	}

	const FString ContextString;
	const FSB_ShipData* const ShipData = ShipDT->FindRow<FSB_ShipData>(RowName, ContextString);
	
	return ShipData;
}

const FSB_BaseModuleData* const USB_GameInstance::GetBaseModuleDataFromRow(const FName& RowName) const
{
	if (BaseModuleDT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetShipDataFromRow : Missing BaseModuleDT."));
		return nullptr;
	}
	
	const FString ContextString;
	const FSB_BaseModuleData* BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(RowName, ContextString);
	if (BaseModuleData)
	{
		return BaseModuleData;
	}

	return nullptr;
}

void USB_GameInstance::Debug()
{
	if (SaveGame)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SaveGame // PlayerName : " + SaveGame->PlayerName);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SaveGame // MapName : " + SaveGame->MapName);
	}
}

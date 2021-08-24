#include "SB_GameInstance.h"
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
		SaveGame->VehicleDataRowName = NewSelectedShipName;
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

const FSB_VehicleData* const USB_GameInstance::GetVehicleDataFromRow(const FName& RowName) const
{
	if (VehicleDT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USB_GameInstance::GetVehicleDataFromRow : Missing VehicleDT."));
		return nullptr;
	}

	const FString ContextString;
	const FSB_VehicleData* const VehicleData = VehicleDT->FindRow<FSB_VehicleData>(RowName, ContextString);
	
	return VehicleData;
}

const FSB_BaseModuleData* const USB_GameInstance::GetBaseModuleDataFromRow(const FName& RowName) const
{
	if (BaseModuleDT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetBaseModuleDataFromRow : Missing BaseModuleDT."));
		return nullptr;
	}
	
	const FString ContextString;
	const FSB_BaseModuleData* BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(RowName, ContextString);
	
	return BaseModuleData;
}

const FSB_ThrusterModuleData* const USB_GameInstance::GetThrusterModuleDataFromRow(const FName& RowName) const
{
	if (ThrusterModuleDT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetThrusterModuleDataFromRow : Missing ThrusterModuleDT."));
		return nullptr;
	}
	
	const FString ContextString;
	const FSB_ThrusterModuleData* const ThrusterModuleData = ThrusterModuleDT->FindRow<FSB_ThrusterModuleData>(RowName, ContextString);
	
	return ThrusterModuleData;
}

const FSB_WeaponModuleData* const USB_GameInstance::GetWeaponModuleDataFromRow(const FName& RowName) const
{
	if (WeaponModuleDT == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetWeaponModuleDataFromRow : Missing WeaponModuleDT."));
		return nullptr;
	}

	const FString ContextString;
	const FSB_WeaponModuleData* WeaponModuleData = WeaponModuleDT->FindRow<FSB_WeaponModuleData>(RowName, ContextString);
	
	return WeaponModuleData;
}

void USB_GameInstance::ToggleDebug(bool bEnable)
{
	DebugSettings.bIsDebugEnabled_WeaponModule = bEnable;
}

void USB_GameInstance::Debug()
{
	if (SaveGame)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SaveGame // PlayerName : " + SaveGame->PlayerName);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "SaveGame // MapName : " + SaveGame->MapName);
	}
}

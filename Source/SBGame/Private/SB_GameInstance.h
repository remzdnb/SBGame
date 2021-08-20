///// SB_GameInstance.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SB_GameInstance.generated.h"

class UDataTable;
class USB_PlayerSaveGame;

UCLASS()
class USB_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void StartGameInstance() override;
	virtual void OnStart() override;
	virtual void Init() override;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Data

public:

	const FSB_VehicleData* const GetVehicleDataFromRow(const FName& RowName) const;
	const FSB_ShipData* const GetShipDataFromRow(const FName& RowName) const;
	const FSB_BaseModuleData* const GetBaseModuleDataFromRow(const FName& RowName) const;
	const FSB_ThrusterModuleData* const GetThrusterModuleDataFromRow(const FName& RowName) const;
	const FSB_WeaponModuleData* const GetWeaponModuleDataFromRow(const FName& RowName) const;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* VehicleDT;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ShipDT;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* BaseModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ThrusterModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponModuleDT;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_ShipSettings ShipSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_ShieldSettings ShieldSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_UISettings UISettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_AISettings AISettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_DebugSettings DebugSettings;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Save Game

public:

	UFUNCTION()
	void CreateNewSaveGame();

	UFUNCTION()
	void SavePlayerName(const FString& NewPlayerName);

	UFUNCTION()
	void SaveSelectedShip(const FName& NewSelectedShipName);
	
	UFUNCTION()
	void SaveShipConfig(const TArray<FName>& NewShipConfig);

	UFUNCTION()
	void ResetShipConfig();

	//

	FORCEINLINE UFUNCTION() const USB_PlayerSaveGame* const GetSaveGame() const { return SaveGame; }

private:

	UPROPERTY()
	USB_PlayerSaveGame* SaveGame;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////

private:

	UFUNCTION()
	void Debug();
	
};

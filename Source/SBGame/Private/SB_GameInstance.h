///// SB_GameInstance.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SB_GameInstance.generated.h"

class ASB_DataManager;
class USB_PlayerSaveGame;

UCLASS()
class USB_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void StartGameInstance() override;
	virtual void OnStart() override;
	virtual void Init() override;

	//

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
	
	const FSB_ShipData* const GetShipDataFromRow(const FName& RowName) const;
	const FSB_BaseModuleData* const GetBaseModuleDataFromRow(const FName& RowName) const;
	
	//
	
	FORCEINLINE UFUNCTION() const USB_PlayerSaveGame* const GetSaveGame() const { return SaveGame; }

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* ShipDT;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* BaseModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_UISettings UISettings;

private:

	UFUNCTION()
	void Debug();

	//
	
	UPROPERTY()
	USB_PlayerSaveGame* SaveGame;
};

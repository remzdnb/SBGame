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
	void ApplySaveGame();

	UFUNCTION()
	void SetNewShipConfig(const TArray<FName>& NewShipConfig);

	//

	const FSB_BaseModuleData* const GetBaseModuleDataFromRow(const FName& RowName) const;
	
	//

	FORCEINLINE UFUNCTION() ASB_DataManager* const GetDataManager() const { return DataManager; }
	FORCEINLINE UFUNCTION() USB_PlayerSaveGame* const GetSaveGame() const { return SaveGame; }

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDataTable* BaseModuleDT;

private:
	
	UPROPERTY()
	ASB_DataManager* DataManager;

	UPROPERTY()
	USB_PlayerSaveGame* SaveGame;

	//

	//

	UFUNCTION()
	void Debug();
};

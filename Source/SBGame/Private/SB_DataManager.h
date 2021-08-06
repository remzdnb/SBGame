#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "SB_DataManager.generated.h"

class UDataTable;

UCLASS(Blueprintable)
class ASB_DataManager : public AInfo
{
	GENERATED_BODY()

public:

	void Lol();

	const FSB_ShipData* const GetShipDataFromRow(const FName& RowName) const;
	const FSB_BaseModuleData& GetBaseModuleDataFromRowRef(const FName& RowName) const;
	const FSB_BaseModuleData* const GetBaseModuleDataFromRow(const FName& RowName) const;
	const FSB_ThrusterModuleData* const GetThrusterModuleDataFromRow(const FName& RowName) const;
	const FSB_BaseWeaponModuleData* const GetWeaponModuleDataFromRow(const FName& RowName) const;
	const FSB_ProjectileData* const GetProjectileDataFromRow(const FName& RowName) const;

	// Settings
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_GameSettings GameSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_AISettings AISettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_UISettings UISettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_ShipSettings ShipSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_ShieldSettings ShieldSettings;

	// DataTables
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable*	ShipDT;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* BaseModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ThrusterModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ProjectileDT;

private:

	FSB_BaseModuleData DefaultBaseModuleData;
};

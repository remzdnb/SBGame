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

	FORCEINLINE UFUNCTION() const UDataTable* const GetBaseModuleDT() { return BaseModuleDT; }
	FORCEINLINE UFUNCTION() const UDataTable* const GetCarrierShipConfigDT() const { return CarrierShipConfigDT; }

	const FSB_BaseModuleData* const GetBaseModuleDataFromRow(FName RowName) const;
	const FSB_ThrusterModuleData* const GetThrusterModuleDataFromRow(const FName& RowName) const;
	const FSB_BaseWeaponModuleData* const GetWeaponModuleDataFromRow(const FName& RowName) const;
	const FSB_ProjectileData* const GetProjectileDataFromRow(FName RowName) const;
	const FSB_ModuleSlotData* const GetCarrierModuleSlotFromRow(const FName& RowName) const;

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

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* BaseModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ThrusterModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* ProjectileDT;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* CarrierShipConfigDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* CargoShipConfigDT;
};

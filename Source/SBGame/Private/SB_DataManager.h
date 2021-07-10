#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "SB_DataManager.generated.h"

UCLASS(Blueprintable)
class ASB_DataManager : public AInfo
{
	GENERATED_BODY()

public:

	FSB_BaseModuleData* const GetBaseModuleDataFromRow(FName RowName) const;
	const FSB_ThrusterModuleData* const GetThrusterModuleDataFromRow(const FName& RowName) const;
	const FSB_WeaponModuleData* const GetWeaponModuleDataFromRow(const FName& RowName) const;
	const FSB_ProjectileData* const GetProjectileDataFromRow(FName RowName) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_GameSettings GameSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_UISettings UISettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_CameraSettings CameraSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_ShipSettings ShipSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSB_ShieldSettings ShieldSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* BaseModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* ThrusterModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* WeaponModuleDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* ProjectileDT;
};

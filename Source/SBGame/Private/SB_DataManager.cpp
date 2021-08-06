#include "SB_DataManager.h"

void ASB_DataManager::Lol()
{
}

const FSB_ShipData* const ASB_DataManager::GetShipDataFromRow(const FName& RowName) const
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

const FSB_BaseModuleData& ASB_DataManager::GetBaseModuleDataFromRowRef(const FName& RowName) const
{
	if (BaseModuleDT == nullptr)
		return DefaultBaseModuleData;

	const FString ContextString;
	const FSB_BaseModuleData* BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(RowName, ContextString);
	if (BaseModuleData)
		return *BaseModuleData;

	return DefaultBaseModuleData;
}

const FSB_BaseModuleData* const ASB_DataManager::GetBaseModuleDataFromRow(const FName& RowName) const
{
	if (BaseModuleDT == nullptr)
		return nullptr;

	const FString ContextString;
	const FSB_BaseModuleData* BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(RowName, ContextString);
	if (BaseModuleData)
	{
		return BaseModuleData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetBaseModuleDataFromRow - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}

	return nullptr;
}

const FSB_ThrusterModuleData* const ASB_DataManager::GetThrusterModuleDataFromRow(const FName& RowName) const
{
	if (ThrusterModuleDT == nullptr)
		return nullptr;

	const FString ContextString;
	const FSB_ThrusterModuleData* const ThrusterModuleData = ThrusterModuleDT->FindRow<FSB_ThrusterModuleData>(RowName, ContextString);
	if (ThrusterModuleData)
	{
		return ThrusterModuleData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetThrusterModuleDataFromRow - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}
}

const FSB_BaseWeaponModuleData* const ASB_DataManager::GetWeaponModuleDataFromRow(const FName& RowName) const
{
	if (WeaponModuleDT == nullptr)
		return nullptr;

	const FString ContextString;
	const FSB_BaseWeaponModuleData* WeaponModuleData = WeaponModuleDT->FindRow<FSB_BaseWeaponModuleData>(RowName, ContextString);
	if (WeaponModuleData)
	{
		return WeaponModuleData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetWeaponModuleDataFromRow - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}
}

const FSB_ProjectileData* const ASB_DataManager::GetProjectileDataFromRow(const FName& RowName) const
{
	if (ProjectileDT == nullptr)
		return nullptr;

	const FString ContextString;
	const FSB_ProjectileData* const ProjectileData = ProjectileDT->FindRow<FSB_ProjectileData>(RowName, ContextString);
	
	return ProjectileData;
}
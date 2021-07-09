#include "SB_DataManager.h"

FSB_BaseModuleData* const ASB_DataManager::GetBaseModuleDataFromRow(FName RowName) const
{
	if (BaseModuleDT == nullptr)
		return nullptr;

	FString ContextString;
	FSB_BaseModuleData* BaseModuleData = BaseModuleDT->FindRow<FSB_BaseModuleData>(RowName, ContextString);
	if (BaseModuleData)
	{
		return BaseModuleData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetBaseModuleDataFromRoww - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}
}

const FSB_ThrusterModuleData* const ASB_DataManager::GetThrusterModuleDataFromRow(const FName& RowName) const
{
	if (ThrusterModuleDT == nullptr)
		return nullptr;

	FString ContextString;
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

const FSB_WeaponModuleData* const ASB_DataManager::GetWeaponModuleDataFromRow(const FName& RowName) const
{
	if (WeaponModuleDT == nullptr)
		return nullptr;

	FString ContextString;
	FSB_WeaponModuleData* WeaponModuleData = WeaponModuleDT->FindRow<FSB_WeaponModuleData>(RowName, ContextString);
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

const FSB_ProjectileData* const ASB_DataManager::GetProjectileDataFromRow(FName RowName) const
{
	if (ProjectileDT == nullptr)
		return nullptr;

	FString ContextString;
	const FSB_ProjectileData* const ProjectileData = ProjectileDT->FindRow<FSB_ProjectileData>(RowName, ContextString);
	if (ProjectileData)
	{
		return ProjectileData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASB_DataManager::GetProjectileDataFromRow - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}
}
#include "Battle/SB_HUDMainWidget.h"
#include "Battle/SB_HUDWeaponWidget.h"
#include "SB_SpectatorWidget.h"
#include "Vehicle/SB_Vehicle.h"
#include "Vehicle/SB_ShipMovementComponent.h"
#include "Module/SB_ThrusterModule.h"
#include "Module/SB_ShieldModule.h"
#include "Module/Weapon/SB_WeaponModule.h"
#include "SB_GameInstance.h"
#include "SB_PlayerController.h"
//
#include "RZ_ProgressBarWidget.h"
#include "RZ_UtilityLibrary.h"
//
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "EngineUtils.h"

void USB_HUDMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	
	OwnerPC = Cast<ASB_PlayerController>(GetOwningPlayer());
	if (OwnerPC)
	{
		// If ships references are already set, initialize now. If not, wait for PlayerController events.

		if (OwnerPC->GetOwnedVehicle())
			OnNewOwnedVehicle(OwnerPC->GetOwnedVehicle());

		OwnerPC->OnNewOwnedVehicle.AddUniqueDynamic(this, &USB_HUDMainWidget::OnNewOwnedVehicle);
	}
}

void USB_HUDMainWidget::OnNewOwnedVehicle(ASB_Vehicle* const NewOwnedVehicle)
{
	if (NewOwnedVehicle == nullptr)
		return;

	OnVehicleDurabilityUpdated(NewOwnedVehicle->GetDurability(), NewOwnedVehicle->GetVehicleData()->MaxDurability);
	NewOwnedVehicle->OnVehicleDurabilityUpdated.AddUniqueDynamic(this, &USB_HUDMainWidget::OnVehicleDurabilityUpdated);

	/*OnShieldDurabilityUpdated(NewOwnedVehicle->ShieldModule->GetShieldDurability(), DataManager->ShieldSettings.MaxDurability);
	NewOwnedVehicle->ShieldModule->OnShieldDurabilityUpdated.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShieldDurabilityUpdated);

	OnShieldCooldownUpdated(1.0f, 1.0f);
	NewOwnedVehicle->ShieldModule->OnShieldCooldownUpdated.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShieldCooldownUpdated);
	
	NewOwnedVehicle->OnDestroyed.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShipDestroyedBPI);

	//

	ThrusterModulesContainer->ClearChildren();
	for (auto& ThrusterModule : NewOwnedVehicle->GetThrusterModules())
	{
		USB_ModuleWidget* ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), DataManager->UISettings.Module_WBP);
		if (ModuleWidget)
		{
			//ModuleWidget->Update(Cast<USB_BaseModule>(ThrusterModule));
			ThrusterModulesContainer->AddChild(ModuleWidget);
		}
	}*/
	
	WeaponModulesContainer->ClearChildren();
	for (const auto& WeaponModule : NewOwnedVehicle->GetWeapons())
	{
		USB_HUDWeaponWidget* WeaponWidget = CreateWidget<USB_HUDWeaponWidget>(GetWorld(), GInstance->UISettings.HUDWeapon_WBP);
		if (WeaponWidget)
		{
			WeaponWidget->Update(WeaponModule->GetModuleSlotData(), WeaponModule);
			WeaponModulesContainer->AddChild(WeaponWidget);
		}
	}

	/*if (NewOwnedVehicle->ShieldModule)
	{
		//ShieldModuleWidget->Update(Cast<USB_BaseModule>(NewOwnedVehicle->ShieldModule));
	}*/

	OnShipSpawnedBPI();
}

void USB_HUDMainWidget::OnVehicleDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShipDurabilityProgressBar->Update(NewDurability, MaxDurability);
}

void USB_HUDMainWidget::OnShieldDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShieldDurabilityProgressBar->Update(NewDurability, MaxDurability);
}

void USB_HUDMainWidget::OnShieldCooldownUpdated(float RemainingTime, float MaxTime)
{
	ShieldCooldownProgressBar->Update(RemainingTime, MaxTime);
}

#include "Battle/SB_HUDMainWidget.h"
#include "Battle/SB_HUDWeaponWidget.h"
#include "SB_SpectatorWidget.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
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

		if (OwnerPC->GetOwnedShip())
			OnNewOwnedShip(OwnerPC->GetOwnedShip());

		OwnerPC->OnNewOwnedShip.AddUniqueDynamic(this, &USB_HUDMainWidget::OnNewOwnedShip);
	}
}

void USB_HUDMainWidget::OnNewOwnedShip(ASB_Ship* const NewOwnedShip)
{
	if (NewOwnedShip == nullptr)
		return;

	/*OnShipDurabilityUpdated(NewOwnedShip->GetDurability(), DataManager->ShipSettings.MaxDurability);
	NewOwnedShip->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShipDurabilityUpdated);

	OnShieldDurabilityUpdated(NewOwnedShip->ShieldModule->GetShieldDurability(), DataManager->ShieldSettings.MaxDurability);
	NewOwnedShip->ShieldModule->OnShieldDurabilityUpdated.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShieldDurabilityUpdated);

	OnShieldCooldownUpdated(1.0f, 1.0f);
	NewOwnedShip->ShieldModule->OnShieldCooldownUpdated.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShieldCooldownUpdated);
	
	NewOwnedShip->OnDestroyed.AddUniqueDynamic(this, &USB_HUDMainWidget::OnShipDestroyedBPI);

	//

	ThrusterModulesContainer->ClearChildren();
	for (auto& ThrusterModule : NewOwnedShip->GetThrusterModules())
	{
		USB_ModuleWidget* ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), DataManager->UISettings.Module_WBP);
		if (ModuleWidget)
		{
			//ModuleWidget->Update(Cast<USB_BaseModule>(ThrusterModule));
			ThrusterModulesContainer->AddChild(ModuleWidget);
		}
	}*/
	
	WeaponModulesContainer->ClearChildren();
	for (const auto& WeaponModule : NewOwnedShip->GetWeapons())
	{
		USB_HUDWeaponWidget* WeaponWidget = CreateWidget<USB_HUDWeaponWidget>(GetWorld(), GInstance->UISettings.HUDWeapon_WBP);
		if (WeaponWidget)
		{
			WeaponWidget->Update(WeaponModule->GetModuleSlotData(), WeaponModule);
			WeaponModulesContainer->AddChild(WeaponWidget);
		}
	}

	/*if (NewOwnedShip->ShieldModule)
	{
		//ShieldModuleWidget->Update(Cast<USB_BaseModule>(NewOwnedShip->ShieldModule));
	}*/

	OnShipSpawnedBPI();
}

void USB_HUDMainWidget::OnShipDurabilityUpdated(float NewDurability, float MaxDurability)
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

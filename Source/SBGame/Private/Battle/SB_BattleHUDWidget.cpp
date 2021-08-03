#include "Battle/SB_BattleHUDWidget.h"
#include "Module/SB_ModuleWidget.h"
#include "SB_SpectatorWidget.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipMovementComponent.h"
#include "Module/SB_ThrusterModule.h"
#include "Module/SB_ShieldModule.h"
#include "Module/Weapon/SB_BaseWeaponModule.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
#include "RZ_ProgressBarWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "EngineUtils.h"

void USB_BattleHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}

	OwnerPC = Cast<ASB_PlayerController>(GetOwningPlayer());
	if (OwnerPC)
	{
		// If ships references are already set, initialize now. If not, wait for PlayerController events.

		if (OwnerPC->GetOwnedShip())
			OnNewOwnedShip(OwnerPC->GetOwnedShip());

		OwnerPC->OnNewOwnedShip.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnNewOwnedShip);
	}
}

void USB_BattleHUDWidget::OnNewOwnedShip(ASB_Ship* const NewOwnedShip)
{
	if (NewOwnedShip == nullptr)
		return;

	OnShipDurabilityUpdated(NewOwnedShip->GetDurability(), DataManager->ShipSettings.MaxDurability);
	NewOwnedShip->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnShipDurabilityUpdated);

	OnShieldDurabilityUpdated(NewOwnedShip->ShieldModule->GetShieldDurability(), DataManager->ShieldSettings.MaxDurability);
	NewOwnedShip->ShieldModule->OnShieldDurabilityUpdated.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnShieldDurabilityUpdated);

	OnShieldCooldownUpdated(1.0f, 1.0f);
	NewOwnedShip->ShieldModule->OnShieldCooldownUpdated.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnShieldCooldownUpdated);
	
	NewOwnedShip->OnDestroyed.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnShipDestroyedBPI);

	//

	ThrusterModulesContainer->ClearChildren();
	for (auto& ThrusterModule : NewOwnedShip->GetThrusterModules())
	{
		USB_ModuleWidget* ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), DataManager->UISettings.Module_WBP);
		if (ModuleWidget)
		{
			ModuleWidget->Update(Cast<USB_BaseModule>(ThrusterModule));
			ThrusterModulesContainer->AddChild(ModuleWidget);
		}
	}
	
	WeaponModulesContainer->ClearChildren();
	for (auto& WeaponModule : NewOwnedShip->GetWeaponModules())
	{
		USB_ModuleWidget* ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), DataManager->UISettings.Module_WBP);
		if (ModuleWidget)
		{
			ModuleWidget->Update(Cast<USB_BaseModule>(WeaponModule));
			WeaponModulesContainer->AddChild(ModuleWidget);
		}
	}

	if (NewOwnedShip->ShieldModule)
	{
		ShieldModuleWidget->Update(Cast<USB_BaseModule>(NewOwnedShip->ShieldModule));
	}

	OnShipSpawnedBPI();
}

void USB_BattleHUDWidget::OnShipDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShipDurabilityProgressBar->Update(NewDurability, DataManager->ShipSettings.MaxDurability);
}

void USB_BattleHUDWidget::OnShieldDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShieldDurabilityProgressBar->Update(NewDurability, MaxDurability);
}

void USB_BattleHUDWidget::OnShieldCooldownUpdated(float RemainingTime, float MaxTime)
{
	ShieldCooldownProgressBar->Update(RemainingTime, MaxTime);
}

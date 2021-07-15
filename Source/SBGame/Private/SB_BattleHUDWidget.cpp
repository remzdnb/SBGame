#include "SB_BattleHUDWidget.h"
#include "SB_ModuleWidget.h"
#include "SB_SpectatorWidget.h"
#include "SB_Ship.h"
#include "SB_ShipMovementComponent.h"
#include "SB_ThrusterModule.h"
#include "SB_ShieldModule.h"
#include "SB_WeaponModule.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
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

	NewOwnedShip->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnShipDurabilityUpdated);
	NewOwnedShip->OnDestroyed.AddUniqueDynamic(this, &USB_BattleHUDWidget::OnShipDestroyedBPI);

	//

	ThrusterModulesContainer->ClearChildren();
	for (auto& ThrusterModule : NewOwnedShip->GetThrusterModules())
	{
		USB_ModuleWidget* ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), DataManager->UISettings.Module_WBP);
		if (ModuleWidget)
		{
			ModuleWidget->Init(DataManager, NewOwnedShip, Cast<USB_BaseModule>(ThrusterModule));
			ThrusterModulesContainer->AddChild(ModuleWidget);
		}
	}
	
	WeaponModulesContainer->ClearChildren();
	for (auto& WeaponModule : NewOwnedShip->GetWeaponModules())
	{
		USB_ModuleWidget* ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), DataManager->UISettings.Module_WBP);
		if (ModuleWidget)
		{
			ModuleWidget->Init(DataManager, NewOwnedShip, Cast<USB_BaseModule>(WeaponModule));
			WeaponModulesContainer->AddChild(ModuleWidget);
		}
	}

	if (NewOwnedShip->GetShieldModule())
	{
		ShieldModuleWidget->Init(DataManager, NewOwnedShip, Cast<USB_BaseModule>(NewOwnedShip->GetShieldModule()));
	}

	OnShipSpawnedBPI();
}

void USB_BattleHUDWidget::OnShipDurabilityUpdated(float NewDurability)
{
	//UE_LOG(LogTemp, Display, TEXT("USB_BattleHUDWidget::OnShipDurabilityUpdated %s"), FString::FromInt(NewDurability));

	ShipDurabilityProgressBar->SetPercent(NewDurability / DataManager->ShipSettings.MaxDurability);
}

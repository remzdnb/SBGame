// SBGame
#include "Campaign/SB_ShipConfigWidget.h"
#include "Campaign/SB_VehicleSelectionItemWidget.h"
#include "Campaign/SB_SlotSelectionItemWidget.h"
#include "Campaign/SB_ModuleSelectionItemWidget.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "Ship/SB_Ship.h"
#include "Module/SB_ModuleSlotComponent.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
#include "RZ_UtilityLibrary.h"
// Plugins
#include "RZ_UIManager.h"
#include "RZ_ButtonWidget.h"
#include "RZ_StatWidget.h"
#include "RZ_CameraActor.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/WidgetSwitcher.h"

#pragma region +++++ Main ...

void USB_ShipConfigWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	
	OwningPC = Cast<ASB_CampaignPlayerController>(GetOwningPlayer());
	if (OwningPC)
	{
		OnNewOwnedShip(OwningPC->GetOwnedShip());
		
		OwningPC->OnNewOwnedShip.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnNewOwnedShip);
		OwningPC->GetUIManager()->OnNewMenuWidgetSelected.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnNewMenuWidgetSelected);
	}

	VehicleSelectionButton->OnButtonPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnVehicleSelectionButtonPressedBPN);
	SlotSelectionButton->OnButtonPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnSlotSelectionButtonPressedBPN);

	OnVehicleSelectionButtonPressedBPN(0);
}

void USB_ShipConfigWidget::OnNewOwnedShip(ASB_Ship* const NewOwnedShip)
{
	OwnedShip = NewOwnedShip;
	if (OwnedShip == nullptr)
		return;
	
	SelectedSlot = nullptr;

	UpdateVehicleSelectionPanel();
	UpdateSlotSelectionPanel();
	UpdateModuleSelectionPanel();
}

void USB_ShipConfigWidget::OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget)
{
	if (SelectedWidget != this)
	{
		if (SelectedSlot)
		{
			SelectedSlot->GetSpawnedModule()->ToggleHighlight(false);
			SelectedSlot = nullptr;
		}

		UpdateSlotSelectionPanel();
		UpdateModuleSelectionPanel();
	}
}

void USB_ShipConfigWidget::OnVehicleSelectionButtonPressedBPN_Implementation(uint8 ButtonID)
{
	if (SelectedSlot)
	{
		SelectedSlot->GetSpawnedModule()->ToggleHighlight(false);
		SelectedSlot = nullptr;
	}

	UpdateSlotSelectionPanel();
	UpdateModuleSelectionPanel();
	
	SlotSelectionButton->SetIsSelected_BPN(false);
	VehicleSelectionButton->SetIsSelected_BPN(true);
	VehicleToSlotSwitcher->SetActiveWidgetIndex(0);
}

void USB_ShipConfigWidget::OnSlotSelectionButtonPressedBPN_Implementation(uint8 ButtonID)
{
	VehicleSelectionButton->SetIsSelected_BPN(false);
	SlotSelectionButton->SetIsSelected_BPN(true);
	VehicleToSlotSwitcher->SetActiveWidgetIndex(1);
}

#pragma endregion

#pragma region +++++ Vehicle selection panel ...

void USB_ShipConfigWidget::UpdateVehicleSelectionPanel()
{
	const FSB_VehicleData* const VehicleData = GInstance->GetVehicleDataFromRow(GInstance->GetSaveGame()->ShipDataRowName);
	if (VehicleData)
	{
		SelectedVehicleNameText->SetText(FText::FromString(VehicleData->DisplayName));
		SelectedVehicleImage->SetBrushFromTexture(VehicleData->DisplayTexture);
		SelectedVehicleDurabilityStat->Init(VehicleData->MaxDurability);
		SelectedVehicleMoveSpeedStat->Init(VehicleData->MoveSpeed);
		SelectedVehicleTurnSpeedStat->Init(VehicleData->TurnRate);
	}
	
	VehicleSelectionContainerPanel->ClearChildren();
	for (const auto& VehicleDataRowName : GInstance->VehicleDT->GetRowNames())
	{
		USB_VehicleSelectionItemWidget* const ItemWidget = CreateWidget<USB_VehicleSelectionItemWidget>(
			GetWorld(),
			VehicleSelectionItemWBP
		);
		if (ItemWidget)
		{
			ItemWidget->Update(VehicleDataRowName);
			ItemWidget->OnVehicleSelectionItemPressed.AddDynamic(this, &USB_ShipConfigWidget::OnVehicleSelectionItemPressed);
			
			VehicleSelectionContainerPanel->AddChild(ItemWidget);

			if (VehicleDataRowName == GInstance->GetSaveGame()->ShipDataRowName)
				ItemWidget->OnSelectionUpdatedBPI(true);
		}
	}
}

void USB_ShipConfigWidget::OnVehicleSelectionItemPressed(const FName& VehicleDataRowName)
{
	GInstance->ResetShipConfig();
	OwningPC->SelectShip(VehicleDataRowName);
	UpdateVehicleSelectionPanel();
}

#pragma endregion

#pragma region +++++ Slot selection panel ...

void USB_ShipConfigWidget::UpdateSlotSelectionPanel()
{
	ShipNameText->SetText(FText::FromString(GInstance->GetSaveGame()->ShipDataRowName.ToString()));

	SlotListContainerPanel->ClearChildren();
	for (const auto& ModuleSlot : OwnedShip->GetModuleSlots())
	{
		USB_SlotSelectionItemWidget* const ItemWidget = CreateWidget<USB_SlotSelectionItemWidget>(GetWorld(), SlotSelectionItemWBP);
		if (ItemWidget)
		{
			SlotListContainerPanel->AddChild(ItemWidget);
			
			ItemWidget->Update(ModuleSlot->GetModuleSlotData(), ModuleSlot->GetSpawnedModule());
			ItemWidget->OnSlotSelectionItemPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnSlotSelectionItemPressed);

            if (SelectedSlot)
            {			
                if (ModuleSlot->GetModuleSlotData().UniqueID == SelectedSlot->GetModuleSlotData().UniqueID)
             		ItemWidget->OnSelectionUpdatedBPI(true);
            }
		}
	}
}

void USB_ShipConfigWidget::OnSlotSelectionItemPressed(uint8 SlotID)
{
	if (SelectedSlot)
	{
		SelectedSlot->GetSpawnedModule()->ToggleHighlight(false);
	}
	
	SelectedSlot = OwnedShip->GetModuleSlots()[SlotID];
	SelectedSlot->GetSpawnedModule()->ToggleHighlight(true);

	UpdateSlotSelectionPanel();
	UpdateModuleSelectionPanel();
	OnModuleListOpenedBPI();

	OwningPC->GetCameraActor()->SetNewLocation(SelectedSlot->GetSpawnedModule()->GetComponentLocation(), true);
}

#pragma endregion

#pragma region +++++ Module selection panel ...

void USB_ShipConfigWidget::UpdateModuleSelectionPanel()
{
	if (SelectedSlot == nullptr)
	{
		ModuleListMainPanel->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	ModuleListMainPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	ShopSlotTypeText->SetText(FText::FromString(RZ_UtilityLibrary::GetEnumAsString("ESB_ModuleType", SelectedSlot->GetModuleSlotData().Type)));
	ShopModuleNameText->SetText(FText::FromString(SelectedSlot->GetSpawnedModule()->GetBaseModuleData()->DisplayName.ToString()));
	ShopModuleImage->SetBrushFromTexture(SelectedSlot->GetSpawnedModule()->GetBaseModuleData()->DisplayTexture);
	
	ModuleListContainerPanel->ClearChildren();

	if (SelectedSlot->GetModuleSlotData().Type != ESB_ModuleType::Weapon)
	{
		return;
	}
	
	for (auto& RowName : GInstance->BaseModuleDT->GetRowNames())
	{
		const FSB_BaseModuleData* const BaseModuleData = GInstance->GetBaseModuleDataFromRow(RowName);
		if (BaseModuleData)
		{
			if (BaseModuleData->ModuleType == SelectedSlot->GetModuleSlotData().Type ||
				BaseModuleData->DisplayName == "Empty")
			{
				USB_ModuleSelectionItemWidget* const ItemWidget = CreateWidget<USB_ModuleSelectionItemWidget>(GetWorld(), ModuleSelectionItemWBP);
				if (ItemWidget)
				{
					ItemWidget->Update(RowName);
					ItemWidget->OnModuleSelectionItemPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnModuleSelectionItemPressed);
					
					ModuleListContainerPanel->AddChild(ItemWidget);

					if (SelectedSlot->GetSpawnedModule()->GetModuleRowName() == RowName)
					{
						ItemWidget->OnSelectionUpdatedBPI(true);
					}
				}
			}
		}
	}
}

void USB_ShipConfigWidget::OnModuleSelectionItemPressed(const FName& ModuleDataRowName)
{
	SelectedSlot->SpawnModule(ModuleDataRowName, true);
	SelectedSlot->GetSpawnedModule()->ToggleHighlight();

	OwnedShip->SaveConfig();
	
	UpdateSlotSelectionPanel();
	UpdateModuleSelectionPanel();
}

#pragma endregion
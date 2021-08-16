// SBGame
#include "Campaign/SB_ShipConfigWidget.h"
#include "Campaign/SB_ShipConfig_Slot_Widget.h"
#include "Campaign/SB_ModuleListItemWidget.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "Module/SB_ModuleWidget.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipCameraManager.h"
#include "Module/SB_BaseModule.h"
#include "SB_DataManager.h"
#include "SB_GameInstance.h"
#include "SB_PlayerSaveGame.h"
#include "SB_UtilityLibrary.h"
// UIPlugin
#include "RZ_ButtonWidget.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EngineUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Module/SB_ModuleSlotComponent.h"

#pragma region +++++ Main ...

void USB_ShipConfigWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	
	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}
	
	OwningPC = Cast<ASB_CampaignPlayerController>(GetOwningPlayer());
	if (OwningPC)
	{
		// If ships references are already set, initialize now. If not, wait for PlayerController events.

		//OwnedShip = OwningPC->GetOwnedShip();
		OnNewOwnedShip(OwningPC->GetOwnedShip());
		OwningPC->OnNewOwnedShip.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnNewOwnedShip);
	}

	PrevShipButton->OnPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnPrevShipButtonPressed);
	NextShipButton->OnPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnNextShipButtonPressed);
}

void USB_ShipConfigWidget::OnNewOwnedShip(ASB_Ship* const NewOwnedShip)
{
	OwnedShip = NewOwnedShip;
	if (OwnedShip == nullptr)
		return;

	DefaultArmRotation = FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 110.0f, 0.0f);
	DefaultArmLength = 20000.0f;
	SelectedArmRotation = FRotator(-45.0f, OwnedShip->GetActorRotation().Yaw - 145.0f, 0.0f);
	SelectedArmLength = 2500.0f;
	
	UpdateModuleList();
	UpdateSlotList();
}

#pragma endregion

#pragma region +++++ Ship selection ...

void USB_ShipConfigWidget::OnPrevShipButtonPressed()
{
	const FName& SavedShipRowName = GInstance->GetSaveGame()->ShipDataRowName;

	for (int32 Index = 0; Index != DataManager->ShipDT->GetRowNames().Num(); ++Index)
	{
		if (DataManager->ShipDT->GetRowNames()[Index] == SavedShipRowName)
		{
			if (DataManager->ShipDT->GetRowNames().IsValidIndex(Index - 1) == false)
			{
				OwningPC->SelectShip(DataManager->ShipDT->GetRowNames()[DataManager->ShipDT->GetRowNames().Num() - 1]);
				return;
			}

			GInstance->ResetShipConfig();
			OwningPC->SelectShip(DataManager->ShipDT->GetRowNames()[Index - 1]);
			return;
		}
	}
}

void USB_ShipConfigWidget::OnNextShipButtonPressed()
{
	const FName& SavedShipRowName = GInstance->GetSaveGame()->ShipDataRowName;
	
	for (int32 Index = 0; Index != DataManager->ShipDT->GetRowNames().Num(); ++Index)
	{
		if (DataManager->ShipDT->GetRowNames()[Index] == SavedShipRowName)
		{
			if (DataManager->ShipDT->GetRowNames().IsValidIndex(Index + 1) == false)
			{
				OwningPC->SelectShip(DataManager->ShipDT->GetRowNames()[0]);
				return;
			}

			GInstance->ResetShipConfig();
			OwningPC->SelectShip(DataManager->ShipDT->GetRowNames()[Index + 1]);
			return;
		}
	}
}

#pragma endregion

#pragma region +++++ Slot list ...

void USB_ShipConfigWidget::UpdateSlotList()
{
	ShipNameText->SetText(FText::FromString(GInstance->GetSaveGame()->ShipDataRowName.ToString()));

	SlotListContainerPanel->ClearChildren();

	for (const auto& ModuleSlot : OwnedShip->GetModuleSlots())
	{
		USB_ModuleWidget* const ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), SlotListItemWBP);
		if (ModuleWidget)
		{
			SlotListContainerPanel->AddChild(ModuleWidget);
			
			ModuleWidget->Update(ModuleSlot->GetModuleSlotData(), ModuleSlot->GetSpawnedModule());
			ModuleWidget->OnPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnSlotListItemPressed);

            if (SelectedSlot)
            {			
                if (ModuleSlot->GetModuleSlotData().UniqueID == SelectedSlot->GetModuleSlotData().UniqueID)
             		ModuleWidget->OnSelectionUpdatedBPI(true);
            }
		}
	}
}

void USB_ShipConfigWidget::OnSlotListItemPressed(uint8 SlotID)
{
	SelectedSlot = OwnedShip->GetModuleSlots()[SlotID];

	for (const auto& ModuleSlot : OwnedShip->GetModuleSlots())
	{
		USB_BaseModule* const Module = ModuleSlot->GetSpawnedModule();
		if (Module)
		{
			Module->ToggleHighlight(false);
		}
	}

	USB_BaseModule* const SelectedModule = SelectedSlot->GetSpawnedModule();
	if (SelectedModule)
	{
		SelectedModule->ToggleHighlight(true);
	}

	UpdateSlotList();
	UpdateModuleList();
	OnModuleListOpenedBPI();
}

#pragma endregion

#pragma region +++++ Module list & Module info ...

void USB_ShipConfigWidget::UpdateModuleList()
{
	if (SelectedSlot == nullptr)
	{
		ModuleListMainPanel->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	ModuleListMainPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	ShopSlotTypeText->SetText(FText::FromString(SB_UtilityLibrary::GetEnumAsString("ESB_ModuleType", SelectedSlot->GetModuleSlotData().Type)));
	ShopModuleNameText->SetText(FText::FromString(SelectedSlot->GetSpawnedModule()->GetBaseModuleData()->DisplayName.ToString()));
	ShopModuleImage->SetBrushFromTexture(SelectedSlot->GetSpawnedModule()->GetBaseModuleData()->DisplayTexture);
	
	ModuleListContainerPanel->ClearChildren();

	if (SelectedSlot->GetModuleSlotData().Type != ESB_ModuleType::PrimaryWeapon &&
		SelectedSlot->GetModuleSlotData().Type != ESB_ModuleType::AuxiliaryWeapon)
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
				USB_ModuleListItemWidget* const ModuleListItemWidget = CreateWidget<USB_ModuleListItemWidget>(GetWorld(), ModuleListItemWBP);
				if (ModuleListItemWidget)
				{
					ModuleListItemWidget->Update(RowName);
					ModuleListItemWidget->OnModuleListItemPressed.AddUniqueDynamic(this, &USB_ShipConfigWidget::OnModuleListItemPressed);
					
					ModuleListContainerPanel->AddChild(ModuleListItemWidget);

					if (SelectedSlot->GetSpawnedModule()->GetModuleRowName() == RowName)
					{
						ModuleListItemWidget->OnSelectionUpdatedBPI(true);
					}
				}
			}
		}
	}
}

void USB_ShipConfigWidget::OnModuleListItemPressed(const FName& ModuleDataRowName)
{
	SelectedSlot->SpawnModule(ModuleDataRowName);
	SelectedSlot->GetSpawnedModule()->ToggleHighlight();

	OwnedShip->SaveConfig();
	
	UpdateSlotList();
	UpdateModuleList();
}

#pragma endregion

#pragma region +++++ Slots ...

void USB_ShipConfigWidget::UpdateSlots()
{
	if (OwnedShip.IsValid() == false)
		return;

	//if (OwnedShip->GetSelectedModule()->ModuleID == ModuleRef-)
	
	/*SlotsContainerCanvas->ClearChildren();

	uint8 Index = 0;
	for (auto& ModuleRef : OwnedShip->GetAllModules())
	{
	if (ModuleRef->GetBaseModuleData()->bIsSelectable)
	{
	USB_ShipConfig_Slot_Widget* const SlotWidget = CreateWidget<USB_ShipConfig_Slot_Widget>(GetWorld(), ShipConfig_Slot_WBP);
	if (SlotWidget)
	{
	SlotWidget->Init(OwnedShip.Get(), ModuleRef);
	SlotsContainerCanvas->AddChild(SlotWidget);
	}
	}

	Index++;
	}*/

	//UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfigWidget::UpdateSlots"));
}

#pragma endregion
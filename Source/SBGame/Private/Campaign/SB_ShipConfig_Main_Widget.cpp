#include "Campaign/SB_ShipConfig_Main_Widget.h"
#include "Campaign/SB_ShipConfig_Slot_Widget.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "Module/SB_ModuleWidget.h"
#include "Ship/SB_Ship.h"
#include "Ship/SB_ShipCameraManager.h"
#include "Module/SB_BaseModule.h"
#include "SB_DataManager.h"
#include "SB_UtilityLibrary.h"
//
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "EngineUtils.h"
#include "RZ_ButtonWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

void USB_ShipConfig_Main_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}
	
	OwningPC = Cast<ASB_CampaignPlayerController>(GetOwningPlayer());
	if (OwningPC)
	{
		// If ships references are already set, initialize now. If not, wait for PlayerController events.

		OwnedShip = OwningPC->GetOwnedShip();
		OwningPC->OnNewOwnedShip.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnNewOwnedShip);
	}

	ShopApplyButton->OnButtonPressed.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnShopApplyButtonPressed);
}

void USB_ShipConfig_Main_Widget::OnNewOwnedShip(ASB_Ship* const NewOwnedShip)
{
	OwnedShip = NewOwnedShip;
	if (OwnedShip == nullptr)
		return;

	DefaultArmRotation = FRotator(-25.0f, OwnedShip->GetActorRotation().Yaw - 110.0f, 0.0f);
	DefaultArmLength = 20000.0f;
	SelectedArmRotation = FRotator(-45.0f, OwnedShip->GetActorRotation().Yaw - 145.0f, 0.0f);
	SelectedArmLength = 2500.0f;

	OwnedShip->OnModuleHovered.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnModuleHovered);
	OwnedShip->OnModuleSelected.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnModuleSelectedBPN);
	OwnedShip->OnModuleReplaced.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnModuleReplaced);

	UpdateSlots();
	UpdateConfigList();
	UpdateShop();
}

void USB_ShipConfig_Main_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnedShip.IsValid() == false)
		return;

	/*float XPosition;
	float YPosition;
	GetOwningPlayer()->GetMousePosition(XPosition, YPosition);

	int32 ViewportX;
	int32 ViewportY;
	GetOwningPlayer()->GetViewportSize(ViewportX,ViewportY);
	
	ModuleInfoPanel->SetRenderTranslation(FVector2D(XPosition, YPosition));*/
	
		//
	
	/*FHitResult HitResult;
	GetOwningPlayer()->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.Component.IsValid())
	{
		const FString DebugString = "USB_ShipConfig_Main_Widget::NativeTick : HitResultActor : " + HitResult.Component->GetName();
		GEngine->AddOnScreenDebugMessage(-1, InDeltaTime, FColor::Orange, DebugString);
	}

	HoveredModule = Cast<USB_BaseModule>(HitResult.Component);
	if (HoveredModule.IsValid())
	{
		OwnedShip->HoverModule(HoveredModule->GetModuleID());
	}*/

	//Debug(InDeltaTime);
	
	/*if (SelectedModule.IsValid())
	{
		FVector2D TargetScreenPosition;
		
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
			GetOwningPlayer(), 
			SelectedModule->GetComponentLocation(),
			TargetScreenPosition,
			false);

		TargetScreenPosition.X = TargetScreenPosition.X - ModuleSelectionPanel->GetDesiredSize().X / 2;
		TargetScreenPosition.Y = TargetScreenPosition.Y - ModuleSelectionPanel->GetDesiredSize().Y / 2;
		
		ModuleSelectionPanel->SetRenderTranslation(TargetScreenPosition);
	}*/
}

void USB_ShipConfig_Main_Widget::OnModuleHovered(const USB_BaseModule* const NewHoveredModule)
{
	UpdateSlots();
}

void USB_ShipConfig_Main_Widget::OnModuleSelectedBPN_Implementation(const USB_BaseModule* const NewSelectedModule)
{
	if (NewSelectedModule)
	{
		UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::OnModuleSelectedBPN :: selectedModule true"));
		
		UpdateShop();
		
		ShopMainPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SlotsContainerCanvas->SetVisibility(ESlateVisibility::Collapsed);
		
		OwnedShip->GetShipCameraManager()->SetTargetArmLength(SelectedArmLength);
		OwnedShip->GetShipCameraManager()->SetArmRotation(SelectedArmRotation, true);
		OwnedShip->GetShipCameraManager()->SetArmRelativeLocation(NewSelectedModule->GetRelativeLocation());
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::OnModuleSelectedBPN :: selectedModule false"));
		
		UpdateSlots();
		
		ShopMainPanel->SetVisibility(ESlateVisibility::Hidden);
		SlotsContainerCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		OwnedShip->GetShipCameraManager()->SetTargetArmLength(DefaultArmLength);
		OwnedShip->GetShipCameraManager()->SetArmRotation(DefaultArmRotation, true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayer());
	}
}


inline void USB_ShipConfig_Main_Widget::OnModuleReplaced()
{
	UpdateSlots();
	UpdateConfigList();
	UpdateShop();

	//OnModuleSelected(nullptr);
}

void USB_ShipConfig_Main_Widget::UpdateSlots()
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

	UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::UpdateSlots"));
}

void USB_ShipConfig_Main_Widget::UpdateConfigList()
{
	if (OwnedShip.IsValid() == false)
		return;

	ConfigListContainerPanel->ClearChildren();

	for (auto& Module : OwnedShip->GetAllModules())
	{
		USB_ModuleWidget* const ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), ConfigModuleWBP);
		if (ModuleWidget)
		{
			ConfigListContainerPanel->AddChild(ModuleWidget);
			ModuleWidget->OnPressed.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnConfigListModulePressed);
			ModuleWidget->Update(Module);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::UpdateConfigList"));
}

void USB_ShipConfig_Main_Widget::OnConfigListModulePressed(uint8 ModuleID, const FName& DataRowName)
{
	if (OwnedShip.IsValid() == false)
		return;

	OwnedShip->SelectModuleByID(ModuleID);
}

void USB_ShipConfig_Main_Widget::UpdateShop()
{
	if (OwnedShip.IsValid() == false)
		return;

	if (OwnedShip->GetSelectedModule() == nullptr)
		return;

	//

	ShopSlotTypeText->SetText(FText::FromString(SB_UtilityLibrary::GetEnumAsString("ESB_ModuleType", OwnedShip->GetSelectedModule()->GetBaseModuleData()->ModuleType)));
	ShopModuleNameText->SetText(FText::FromString(OwnedShip->GetSelectedModule()->GetBaseModuleData()->DisplayName.ToString()));
	ShopModuleImage->SetBrushFromTexture(OwnedShip->GetSelectedModule()->GetBaseModuleData()->DisplayTexture);

	//
	
	ShopContainerPanel->ClearChildren();
	
	for (auto& RowName : DataManager->GetBaseModuleDT()->GetRowNames())
	{
		const FSB_BaseModuleData* const BaseModuleData = DataManager->GetBaseModuleDataFromRow(RowName);
		if (BaseModuleData)
		{
			if (BaseModuleData->ModuleType == OwnedShip->GetSelectedModule()->GetBaseModuleData()->ModuleType)
			{
				USB_ModuleWidget* const ModuleWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), ShopModuleWBP);
				if (ModuleWidget)
				{
					ModuleWidget->Update(nullptr, OwnedShip->GetSelectedModule()->GetModuleSlotData()->UniqueID, RowName, BaseModuleData);
					ModuleWidget->OnPressed.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnShopModuleSelected);
					ShopContainerPanel->AddChild(ModuleWidget);

					if (OwnedShip->GetSelectedModule()->GetModuleRowName() == RowName)
					{
						ModuleWidget->OnSelectionUpdatedBPI(true);
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::UpdateShop"));
}

void USB_ShipConfig_Main_Widget::OnShopModuleSelected(uint8 NewModuleID, const FName& DataRowName)
{
	if (OwnedShip.IsValid() == false)
		return;

	OwnedShip->ReplaceModule(NewModuleID, DataRowName);
}

void USB_ShipConfig_Main_Widget::OnShopApplyButtonPressed(uint8 ButtonID)
{
	if (OwnedShip.IsValid() == false)
		return;
		
	OwnedShip->SelectModule(nullptr);
}

void USB_ShipConfig_Main_Widget::Debug(float DeltaTime)
{
	FString DebugString = "USB_ShipConfig_Main_Widget::Debug // HoveredModule : ";
	
	/*if (HoveredModule.IsValid())
		DebugString += HoveredModule->GetName() + " // HoveredModuleID : " + FString::FromInt(HoveredModule->GetModuleID());
	else
		DebugString += "nullptr";*/

	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Orange, DebugString);
}
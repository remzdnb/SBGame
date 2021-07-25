#include "Campaign/SB_ShipConfig_Main_Widget.h"
#include "Campaign/SB_ShipConfig_Slot_Widget.h"
#include "Campaign/SB_ShipConfig_Module_Widget.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "SB_ModuleWidget.h"
#include "SB_Ship.h"
#include "SB_ShipCameraManager.h"
#include "SB_BaseModule.h"
#include "SB_DataManager.h"
//
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
	OwnedShip->OnModuleSelected.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnModuleSelected);
	OwnedShip->OnModuleReplaced.AddUniqueDynamic(this, &USB_ShipConfig_Main_Widget::OnModuleReplaced);

	RefreshSlotWidgets();
	RefreshModuleListWidgets();
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
}

void USB_ShipConfig_Main_Widget::OnModuleSelected(const USB_BaseModule* const NewSelectedModule)
{
	if (NewSelectedModule)
	{
		ModuleSelectionMainPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SlotsContainerCanvas->SetVisibility(ESlateVisibility::Collapsed);
		
		OwnedShip->GetShipCameraManager()->SetTargetArmLength(SelectedArmLength);
		OwnedShip->GetShipCameraManager()->SetArmRotation(SelectedArmRotation, true);
		OwnedShip->GetShipCameraManager()->SetArmRelativeLocation(NewSelectedModule->GetRelativeLocation());
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer());
	
		
		UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::OnModuleSelected :: NewModule."));
	}
	else
	{
		ModuleSelectionMainPanel->SetVisibility(ESlateVisibility::Hidden);
		SlotsContainerCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		OwnedShip->GetShipCameraManager()->SetTargetArmLength(DefaultArmLength);
		OwnedShip->GetShipCameraManager()->SetArmRotation(DefaultArmRotation, true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayer());
		
		UE_LOG(LogTemp, Warning, TEXT("USB_ShipConfig_Main_Widget::OnModuleSelected :: New nullptr module."));
	}

	RefreshModuleWidgets();
}


inline void USB_ShipConfig_Main_Widget::OnModuleReplaced()
{
	RefreshSlotWidgets();
	RefreshModuleWidgets();
	RefreshModuleListWidgets();

	//OnModuleSelected(nullptr);
}

void USB_ShipConfig_Main_Widget::RefreshSlotWidgets()
{
	SlotsContainerCanvas->ClearChildren();

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
	}
}

void USB_ShipConfig_Main_Widget::RefreshModuleWidgets()
{
	if (OwnedShip.IsValid() == false)
		return;

	if (OwnedShip->GetSelectedModule() == nullptr)
		return;

	// selected module valid ?
	
	ModuleSelectionContainerPanel->ClearChildren();
	ModuleWidgets.Empty();
	
	for (auto& RowName : DataManager->GetBaseModuleDT()->GetRowNames())
	{
		FSB_BaseModuleData* const BaseModuleData = DataManager->GetBaseModuleDataFromRow(RowName);
		if (BaseModuleData)
		{
			if (BaseModuleData->ModuleType == OwnedShip->GetSelectedModule()->GetBaseModuleData()->ModuleType)
			{
				USB_ShipConfig_Module_Widget* const ModuleWidget = CreateWidget<USB_ShipConfig_Module_Widget>(GetWorld(), ShipConfig_Module_WBP);
				if (ModuleWidget)
				{
					ModuleWidget->Init(DataManager, OwnedShip.Get(), OwnedShip->GetSelectedModule()->ModuleID, RowName);
					ModuleSelectionContainerPanel->AddChild(ModuleWidget);
				}
			}
		}
	}
}

void USB_ShipConfig_Main_Widget::RefreshModuleListWidgets()
{
	if (OwnedShip.IsValid() == false)
		return;

	ModuleListContainerPanel->ClearChildren();

	for (auto& Module : OwnedShip->GetAllModules())
	{
		USB_ModuleWidget* const ModuleSlotWidget = CreateWidget<USB_ModuleWidget>(GetWorld(), ModuleSlotWBP);
		if (ModuleSlotWidget)
		{
			ModuleListContainerPanel->AddChild(ModuleSlotWidget);
			ModuleSlotWidget->Update(Module);
		}
	}

	UE_LOG(LogTemp, Display, TEXT("USB_ShipConfig_Main_Widget::RefreshModuleListWidgets"));
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

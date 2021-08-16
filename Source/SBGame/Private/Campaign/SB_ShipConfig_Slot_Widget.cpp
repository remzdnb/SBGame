#include "Campaign/SB_ShipConfig_Slot_Widget.h"
#include "Module/SB_BaseModule.h"
#include "Ship/SB_Ship.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USB_ShipConfig_Slot_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USB_ShipConfig_Slot_Widget::Init(class ASB_Ship* const NewShipRef, USB_BaseModule* const NewModuleRef)
{
	ShipRef = NewShipRef;
	ModuleRef = NewModuleRef;
	ModuleName->SetText(FText::FromString(ModuleRef->GetBaseModuleData()->DisplayName.ToString()));
}

void USB_ShipConfig_Slot_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ModuleRef.IsValid())
	{
		FVector2D TargetScreenPosition;
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
			GetOwningPlayer(), 
			ModuleRef->GetComponentLocation(),
			TargetScreenPosition,
			false);
			
		TargetScreenPosition.X = TargetScreenPosition.X - MainPanel->GetDesiredSize().X / 2;
		TargetScreenPosition.Y = TargetScreenPosition.Y - MainPanel->GetDesiredSize().Y / 2;

		MainPanel->SetRenderTranslation(TargetScreenPosition);
	}
}

void USB_ShipConfig_Slot_Widget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	//TargetImage->SetVisibility(ESlateVisibility::Hidden);
	//nfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (ShipRef.IsValid() && ModuleRef.IsValid())
	{
		//ShipRef->HoverModule(ModuleRef->GetModuleID());
	}
}

void USB_ShipConfig_Slot_Widget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	//TargetImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//InfoPanel->SetVisibility(ESlateVisibility::Hidden);
	
	//ShipRef->HoverModule(-1);
}

FReply USB_ShipConfig_Slot_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (ShipRef.IsValid())
	{
		//ShipRef->SelectModule(ModuleRef->GetModuleID());	
	}
	
	return FReply::Handled();
}



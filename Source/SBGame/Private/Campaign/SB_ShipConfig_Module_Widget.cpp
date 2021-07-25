#include "Campaign/SB_ShipConfig_Module_Widget.h"
#include "Campaign/SB_ShipConfig_Main_Widget.h"
#include "SB_Ship.h"
//
#include "SB_DataManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USB_ShipConfig_Module_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USB_ShipConfig_Module_Widget::Init(ASB_DataManager* const NewDataManager, ASB_Ship* const NewShipRef, uint8 NewModuleID, const FName& NewDataRowName)
{
	DataManager = NewDataManager;
	ShipRef = NewShipRef;
	ModuleID = NewModuleID;
	DataRowName = NewDataRowName;

	FSB_BaseModuleData* const BaseModuleData = DataManager->GetBaseModuleDataFromRow(NewDataRowName);
	if (BaseModuleData)
	{
		ModuleNameText->SetText(FText::FromString(BaseModuleData->DisplayName.ToString()));
		ModuleThumbnailImage->SetBrushFromTexture(BaseModuleData->DisplayTexture);
	}
}

FReply USB_ShipConfig_Module_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (ShipRef == nullptr)
		return FReply::Handled();

	ShipRef->ReplaceModule(ModuleID, DataRowName);

	return FReply::Handled();
}
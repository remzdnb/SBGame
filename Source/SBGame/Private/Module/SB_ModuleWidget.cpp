#include "Module/SB_ModuleWidget.h"
#include "Ship/SB_Ship.h"
#include "Module/SB_BaseModule.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
#include "SB_UtilityLibrary.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void USB_ModuleWidget::Update(
		USB_BaseModule* const ModuleRef,
		uint8 NewModuleID,
		const FName& NewDataRowName,
		const FSB_BaseModuleData* const ModuleData
		)
{
	BaseModuleRef = ModuleRef;
	ModuleID = NewModuleID;
	DataRowName = NewDataRowName;

	if (BaseModuleRef.IsValid())
	{
		BaseModuleRef->OnSelectionUpdated.AddUniqueDynamic(this, &USB_ModuleWidget::OnSelectionUpdatedBPI);
		BaseModuleRef->OnStateUpdated.AddUniqueDynamic(this, &USB_ModuleWidget::OnStateUpdatedBPI);
		BaseModuleRef->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_ModuleWidget::OnDurabilityUpdated);
		
		const FSB_BaseModuleData* const ModuleRefData = BaseModuleRef->GetBaseModuleData();
		if (ModuleRefData)
		{
			ModuleSlotIDText->SetText(FText::FromString(FString::FromInt(BaseModuleRef->GetModuleSlotData()->UniqueID)));
			ModuleSlotTypeText->SetText(FText::FromString(SB_UtilityLibrary::GetEnumAsString("ESB_ModuleType", ModuleRefData->ModuleType)));
			ModuleRowNameText->SetText(FText::FromString(ModuleRefData->DisplayName.ToString()));
			ModuleImage->SetBrushFromTexture(ModuleRefData->DisplayTexture);
		}
	}
	else
	{
		if (ModuleData)
		{
			ModuleSlotIDText->SetText(FText::FromString(FString::FromInt(ModuleID)));
			ModuleSlotTypeText->SetText(FText::FromString(SB_UtilityLibrary::GetEnumAsString("ESB_ModuleType", ModuleData->ModuleType)));
			ModuleRowNameText->SetText(FText::FromString(ModuleData->DisplayName.ToString()));
			ModuleImage->SetBrushFromTexture(ModuleData->DisplayTexture);
		}
	}
}

FReply USB_ModuleWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}

	//

	if (BaseModuleRef.IsValid())
		OnPressed.Broadcast(BaseModuleRef->GetModuleSlotData()->UniqueID , BaseModuleRef->GetModuleRowName());
	else
		OnPressed.Broadcast(ModuleID, DataRowName);

	return FReply::Handled();
}

FReply USB_ModuleWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void USB_ModuleWidget::OnDurabilityUpdated(float NewDurability)
{
	ModuleDurabilityProgressBar->SetPercent(NewDurability / BaseModuleRef->GetBaseModuleData()->MaxDurability);
}
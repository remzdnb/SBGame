#include "SB_ModuleWidget.h"
#include "SB_Ship.h"
#include "SB_ShipPowerComponent.h"
#include "SB_BaseModule.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
#include "SB_UtilityLibrary.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void USB_ModuleWidget::Init(const class ASB_DataManager* const NewDataManager, class ASB_Ship* const NewShipRef, class USB_BaseModule* const NewBaseModuleRef)
{
	DataManager = NewDataManager;
	ShipRef = NewShipRef;
	BaseModuleRef = NewBaseModuleRef;

	ModuleSlotText->SetText(FText::FromString(NewBaseModuleRef->GetParentSlotName().ToString()));
	ModuleClassText->SetText(FText::FromString(NewBaseModuleRef->GetBaseModuleData()->DisplayName.ToString()));
	ModuleImage->SetBrushFromTexture(NewBaseModuleRef->GetBaseModuleData()->DisplayTexture);

	//

	BaseModuleRef->ModuleSelectionUpdatedEvent.AddUniqueDynamic(this, &USB_ModuleWidget::OnSelectionUpdatedBPI);
	BaseModuleRef->ModuleDurabilityUpdatedEvent.AddUniqueDynamic(this, &USB_ModuleWidget::OnDurabilityUpdated);
}

void USB_ModuleWidget::OnDurabilityUpdated(float NewDurability)
{
	ModuleDurabilityProgressBar->SetPercent(NewDurability / BaseModuleRef->GetBaseModuleData()->MaxDurability);
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

	ASB_PlayerController* const OwnerPC = Cast<ASB_PlayerController>(GetOwningPlayer());
	if (OwnerPC)
	{
		//OwnerPC->SetTargetModule(BaseModuleRef);
	}

	return FReply::Handled();
}

FReply USB_ModuleWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}
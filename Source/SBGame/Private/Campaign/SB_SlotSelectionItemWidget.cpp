#include "Campaign/SB_SlotSelectionItemWidget.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameInstance.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

USB_SlotSelectionItemWidget::USB_SlotSelectionItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(UUserWidget::GetWorld()));
}

void USB_SlotSelectionItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply USB_SlotSelectionItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	OnSlotSelectionItemPressed.Broadcast(SlotData.UniqueID);

	return FReply::Handled();

	/*if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}*/
}

FReply USB_SlotSelectionItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void USB_SlotSelectionItemWidget::Update(const FSB_ModuleSlotData& NewSlotData, const USB_BaseModule* const NewModuleRef)
{
	SlotData = NewSlotData;
	
	SlotID->SetText(FText::FromString(FString::FromInt(NewSlotData.UniqueID)));
	SlotName->SetText(FText::FromString(NewSlotData.DisplayName));

	if (NewModuleRef)
	{
		ModuleName->SetText(FText::FromString(NewModuleRef->GetBaseModuleData()->DisplayName.ToString()));
	}
}
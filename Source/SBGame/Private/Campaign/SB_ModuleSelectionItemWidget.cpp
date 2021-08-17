#include "Campaign/SB_ModuleSelectionItemWidget.h"
#include "SB_GameInstance.h"
//
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

FReply USB_ModuleSelectionItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	OnModuleSelectionItemPressed.Broadcast(ModuleDataRowName);

	return FReply::Handled();

	/*if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}*/
}

void USB_ModuleSelectionItemWidget::Update(const FName& NewModuleDataRowName)
{
	ModuleDataRowName = NewModuleDataRowName;
	
	const USB_GameInstance* const GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(UUserWidget::GetWorld()));
	if (GInstance)
	{
		const FSB_BaseModuleData* const BaseModuleData = GInstance->GetBaseModuleDataFromRow(NewModuleDataRowName);
		if (BaseModuleData)
		{
			ModuleNameText->SetText(FText::FromString(BaseModuleData->DisplayName.ToString()));
			ModuleImage->SetBrushFromTexture(BaseModuleData->DisplayTexture);
		}
	}
}
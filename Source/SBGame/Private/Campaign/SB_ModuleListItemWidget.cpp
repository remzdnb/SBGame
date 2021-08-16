#include "Campaign/SB_ModuleListItemWidget.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameInstance.h"
//
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

FReply USB_ModuleListItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	OnModuleListItemPressed.Broadcast(ModuleDataRowName);

	return FReply::Handled();

	/*if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}*/
}

void USB_ModuleListItemWidget::Update(const FName& NewModuleDataRowName)
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
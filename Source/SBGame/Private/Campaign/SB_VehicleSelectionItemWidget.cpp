#include "Campaign/SB_VehicleSelectionItemWidget.h"
#include "SB_GameInstance.h"
//
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

FReply USB_VehicleSelectionItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	OnVehicleSelectionItemPressed.Broadcast(VehicleDataRowName);

	return FReply::Handled();

	/*if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}*/
}

void USB_VehicleSelectionItemWidget::Update(const FName& NewVehicleDataRowName)
{
	VehicleDataRowName = NewVehicleDataRowName;
	
	const USB_GameInstance* const GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(UUserWidget::GetWorld()));
	if (GInstance)
	{
		const FSB_VehicleData* const VehicleData = GInstance->GetVehicleDataFromRow(NewVehicleDataRowName);
		if (VehicleData)
		{
			VehicleNameText->SetText(FText::FromString(VehicleData->DisplayName));
			VehicleImage->SetBrushFromTexture(VehicleData->DisplayTexture);
		}
	}
}
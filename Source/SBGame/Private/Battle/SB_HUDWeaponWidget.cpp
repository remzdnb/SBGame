#include "Battle/SB_HUDWeaponWidget.h"
#include "Module/SB_BaseModule.h"
#include "SB_GameInstance.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

#include "EngineUtils.h"

USB_HUDWeaponWidget::USB_HUDWeaponWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GInstance = Cast<USB_GameInstance>(UGameplayStatics::GetGameInstance(UUserWidget::GetWorld()));
}

void USB_HUDWeaponWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply USB_HUDWeaponWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	OnHUDWeaponWidgetPressed.Broadcast(SlotData.UniqueID);

	return FReply::Handled();

	/*if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
	}*/
}

FReply USB_HUDWeaponWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void USB_HUDWeaponWidget::Update(const FSB_ModuleSlotData& NewSlotData, const USB_BaseModule* const NewModuleRef)
{
	SlotData = NewSlotData;
	ModuleRef = NewModuleRef;
	
	SlotID->SetText(FText::FromString(FString::FromInt(NewSlotData.UniqueID)));

	if (NewModuleRef)
	{
		WeaponName->SetText(FText::FromString(NewModuleRef->GetBaseModuleData()->DisplayName.ToString()));
	}
}
#include "SB_ShipOTMWidget.h"
#include "SB_Ship.h"
#include "SB_UIManager.h"
#include "SB_PlayerController.h"
#include "SB_DataManager.h"
#include "RZ_ProgressBarWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USB_ShipOTMWidget::Init(const ASB_DataManager* const NewDataManager, ASB_Ship* const NewShipRef)
{
	DataManager = NewDataManager;

	ASB_PlayerController* const OwningPC = Cast<ASB_PlayerController>(GetOwningPlayer());
	if (OwningPC)
	{
		OwningPC->GetUIManager()->AddHUDWidget(this);
	}

	ShipRef = NewShipRef;
	ShipRef->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_ShipOTMWidget::Update);
}

void USB_ShipOTMWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ShipRef.IsValid() == false)
		return;

	FVector2D TargetScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), ShipRef->GetActorLocation() + FVector(0.0f, 0.0f, OTM_ZOFFSET), TargetScreenPosition, false);
	TargetScreenPosition.X = TargetScreenPosition.X - MainPanel->GetDesiredSize().X / 2;
	TargetScreenPosition.Y = TargetScreenPosition.Y - MainPanel->GetDesiredSize().Y / 2;

	MainPanel->SetRenderTranslation(TargetScreenPosition);
}

void USB_ShipOTMWidget::Update(float NewDurability)
{
	DurabilityProgressBar->UpdatePercent(NewDurability / DataManager->ShipSettings.MaxDurability);
}
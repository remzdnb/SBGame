#include "Battle/SB_ShipOTMWidget.h"
#include "Ship/SB_Ship.h"
#include "Module/SB_ShieldModule.h"
#include "RZ_UIManager.h"
#include "SB_PlayerController.h"
#include "RZ_ProgressBarWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USB_ShipOTMWidget::Init(ASB_Ship* const NewOwnedShip)
{
	ASB_PlayerController* const OwningPC = Cast<ASB_PlayerController>(GetOwningPlayer());
	if (OwningPC)
	{
		OwningPC->GetUIManager()->AddHUDWidget(this);
	}

	OwnedShip = NewOwnedShip;
	OwnedShip->OnDurabilityUpdated.AddUniqueDynamic(this, &USB_ShipOTMWidget::OnShipDurabilityUpdated);
	//OwnedShip->ShieldModule->OnShieldDurabilityUpdated.AddUniqueDynamic(this, &USB_ShipOTMWidget::OnShieldDurabilityUpdated);
}

void USB_ShipOTMWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnedShip.IsValid() == false)
		return;

	FVector2D TargetScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), OwnedShip->GetActorLocation() + FVector(0.0f, 0.0f, OTM_ZOFFSET), TargetScreenPosition, false);
	TargetScreenPosition.X = TargetScreenPosition.X - MainPanel->GetDesiredSize().X / 2;
	TargetScreenPosition.Y = TargetScreenPosition.Y - MainPanel->GetDesiredSize().Y / 2;

	MainPanel->SetRenderTranslation(TargetScreenPosition);
}

void USB_ShipOTMWidget::OnShipDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShipDurabilityProgressBar->Update(NewDurability, MaxDurability);
}

void USB_ShipOTMWidget::OnShieldDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShieldDurabilityProgressBar->Update(NewDurability, MaxDurability);
}
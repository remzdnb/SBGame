#include "Battle/SB_HUDVehicleOTMWidget.h"
#include "Vehicle/SB_Vehicle.h"
#include "Module/SB_ShieldModule.h"
#include "RZ_UIManager.h"
#include "SB_PlayerController.h"
#include "RZ_ProgressBarWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USB_HUDVehicleOTMWidget::Init(ASB_Vehicle* const NewOwnedVehicle)
{
	ASB_PlayerController* const OwningPC = Cast<ASB_PlayerController>(GetOwningPlayer());
	if (OwningPC && OwningPC->GetUIManager())
	{
		OwningPC->GetUIManager()->AddHUDWidget(this);
	}

	OwnedVehicle = NewOwnedVehicle;
	OwnedVehicle->OnVehicleDurabilityUpdated.AddUniqueDynamic(this, &USB_HUDVehicleOTMWidget::OnVehicleDurabilityUpdated);
	//OwnedVehicle->ShieldModule->OnShieldDurabilityUpdated.AddUniqueDynamic(this, &USB_HUDVehicleOTMWidget::OnShieldDurabilityUpdated);
}

void USB_HUDVehicleOTMWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnedVehicle.IsValid() == false)
		return;

	FVector2D TargetScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), OwnedVehicle->GetActorLocation() + FVector(0.0f, 0.0f, OTM_ZOFFSET), TargetScreenPosition, false);
	TargetScreenPosition.X = TargetScreenPosition.X - MainPanel->GetDesiredSize().X / 2;
	TargetScreenPosition.Y = TargetScreenPosition.Y - MainPanel->GetDesiredSize().Y / 2;

	MainPanel->SetRenderTranslation(TargetScreenPosition);
}

void USB_HUDVehicleOTMWidget::OnVehicleDurabilityUpdated(float NewDurability, float MaxDurability)
{
	VehicleDurabilityPBar->Update(NewDurability, MaxDurability);
}

void USB_HUDVehicleOTMWidget::OnShieldDurabilityUpdated(float NewDurability, float MaxDurability)
{
	ShieldDurabilityPBar->Update(NewDurability, MaxDurability);
}
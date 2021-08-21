#include "SB_CursorWidget.h"
#include "Module/SB_BaseModule.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "Battle/SB_BattlePlayerController.h"
#include "RZ_StatWidget.h"
//
#include "Vehicle/SB_Vehicle.h"
#include "Components/TextBlock.h"

void USB_CursorWidget::NativeOnInitialized()
{
	CampaignPC = Cast<ASB_CampaignPlayerController>(GetOwningPlayer());
	if (CampaignPC)
	{
		CampaignPC->OnNewOwnedVehicle.AddUniqueDynamic(this, &USB_CursorWidget::OnNewOwnedVehicle);
	}
}

void USB_CursorWidget::OnNewOwnedVehicle(ASB_Vehicle* const NewOwnedVehicle)
{
	OwnedVehicle = NewOwnedVehicle;
	if (OwnedVehicle.IsValid())
	{
		//OwnedVehicle->OnModuleHovered.AddUniqueDynamic(this, &USB_CursorWidget::OnHoveredModuleUpdated);
		//OwnedVehicle->OnModuleSelected.AddUniqueDynamic(this, &USB_CursorWidget::OnSelectedModuleUpdated);
	}
}

void USB_CursorWidget::OnHoveredModuleUpdated(const USB_BaseModule* const NewHoveredModule)
{
	if (OwnedVehicle.IsValid() == false)
		return;

	/*if (OwnedVehicle->GetSelectedModule() == nullptr)
	{
		if (NewHoveredModule)
		{
			ModuleInfoNameText->SetText(FText::FromString(NewHoveredModule->GetBaseModuleData()->DisplayName.ToString()));
			ModuleInfoDurabilityStat->Init(NewHoveredModule->GetBaseModuleData()->MaxDurability);
			ModuleInfoArmorStat->Init(NewHoveredModule->GetBaseModuleData()->ShipDamageModifier);
		
			PlayStartAnimationBPI(true);
		}
		else
		{
			PlayStartAnimationBPI(false);
		}
	}
	else
	{
		PlayStartAnimationBPI(false);
	}*/
}

void USB_CursorWidget::OnSelectedModuleUpdated(const USB_BaseModule* const NewSelectedModule)
{
	if (NewSelectedModule)
	{
		PlayStartAnimationBPI(false);
	}
}
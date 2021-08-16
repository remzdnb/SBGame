#include "SB_CursorWidget.h"
#include "Module/SB_BaseModule.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "Battle/SB_BattlePlayerController.h"
#include "RZ_StatWidget.h"
//
#include "Ship/SB_Ship.h"
#include "Components/TextBlock.h"

void USB_CursorWidget::NativeOnInitialized()
{
	CampaignPC = Cast<ASB_CampaignPlayerController>(GetOwningPlayer());
	if (CampaignPC)
	{
		CampaignPC->OnNewOwnedShip.AddUniqueDynamic(this, &USB_CursorWidget::OnNewOwnedShip);
	}
}

void USB_CursorWidget::OnNewOwnedShip(ASB_Ship* const NewOwnedShip)
{
	OwnedShip = NewOwnedShip;
	if (OwnedShip.IsValid())
	{
		//OwnedShip->OnModuleHovered.AddUniqueDynamic(this, &USB_CursorWidget::OnHoveredModuleUpdated);
		//OwnedShip->OnModuleSelected.AddUniqueDynamic(this, &USB_CursorWidget::OnSelectedModuleUpdated);
	}
}

void USB_CursorWidget::OnHoveredModuleUpdated(const USB_BaseModule* const NewHoveredModule)
{
	if (OwnedShip.IsValid() == false)
		return;

	/*if (OwnedShip->GetSelectedModule() == nullptr)
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
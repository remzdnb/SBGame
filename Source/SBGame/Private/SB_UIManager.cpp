//
#include "SB_UIManager.h"
#include "SB_BattleMenuWidget.h"
#include "SB_BattleHUDWidget.h"
#include "SB_DataManager.h"
//
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "EngineUtils.h"

#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

ASB_UIManager::ASB_UIManager() :
	bIsMenuOpen(false),
	bIsHUDOpen(false)
{
}

void ASB_UIManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}
}

void ASB_UIManager::BeginPlay()
{
	Super::BeginPlay();

	BattleMenuWidget = CreateWidget<USB_BattleMenuWidget>(GetWorld(), DataManager->UISettings.BattleMenu_WBP);
	if (BattleMenuWidget == nullptr)
		return;

	BattleHUDWidget = CreateWidget<USB_BattleHUDWidget>(GetWorld(), DataManager->UISettings.BattleHUD_WBP);
	if (BattleHUDWidget == nullptr)
		return;

	for (auto& MapRow : DataManager->UISettings.BattleMenuWidgets)
	{
		UUserWidget* const CreatedWidget = BattleMenuWidget->LoadWidget(MapRow.Value, MapRow.Key);
		if (CreatedWidget)
		{
			MenuWidgets.Add(MapRow.Key, CreatedWidget);
		}
	}

	ToggleHUD(true);
}

void ASB_UIManager::ToggleMenu(bool bNewIsOpen)
{
	if (BattleMenuWidget == nullptr)
		return;

	if (bNewIsOpen == true)
	{
		if (bIsMenuOpen == false)
		{
			BattleMenuWidget->AddToViewport();
			bIsMenuOpen = true;
		}
	}
	else
	{
		if (bIsMenuOpen == true)
		{
			BattleMenuWidget->RemoveFromParent();
			bIsMenuOpen = false;
		}
	}
}

void ASB_UIManager::ToggleHUD(bool bNewIsOpen)
{
	if (BattleHUDWidget == nullptr)
		return;

	if (bNewIsOpen == true)
	{
		if (bIsHUDOpen == false)
		{
			BattleHUDWidget->AddToViewport();
			bIsHUDOpen = true;
		}
	}
	else
	{
		if (bIsHUDOpen == true)
		{
			BattleHUDWidget->RemoveFromParent();
			bIsHUDOpen = false;
		}
	}
}

void ASB_UIManager::AddHUDWidget(UUserWidget* NewWidget)
{
	BattleHUDWidget->GetMainCanvas()->AddChild(NewWidget);

	UCanvasPanelSlot* const CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(NewWidget);
	CanvasPanelSlot->bAutoSize = true;

	ToggleHUD(false);
	ToggleHUD(true); // ?? Refresh
}

void ASB_UIManager::OpenMenuWidgetByName(const FName& WidgetName)
{
	if (BattleMenuWidget && MenuWidgets.FindRef(WidgetName))
	{
		BattleMenuWidget->SetActiveWidgetByRef(MenuWidgets.FindRef(WidgetName));
	}

	ToggleHUD(false);
	ToggleMenu(true);
}

void ASB_UIManager::UpdateCursor()
{

}

void ASB_UIManager::CreateDamagePopup(float Damage, const AActor* const TargetActor)
{
	if (DataManager == nullptr || BattleHUDWidget == nullptr)
		return;

	/*URZ_DamageDealtNotificationWidget* const DDNotificationWidget = CreateWidget<URZ_DamageDealtNotificationWidget>(GetWorld(), DataManager->RZ_DamageDealtNotification_WB);
	if (DDNotificationWidget)
	{
		DDNotificationWidget->Init(TargetActor, Damage);
		//BattleHUDWidget->MainPanel->AddChild(DDNotificationWidget);
	}*/
}

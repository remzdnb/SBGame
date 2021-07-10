//
#include "SB_UIManager.h"
#include "SB_BattleMenuWidget.h"
#include "SB_BattleHUDWidget.h"
#include "SB_DataManager.h"
//
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

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

	MenuLayoutWidget = CreateWidget<USB_BattleMenuWidget>(GetWorld(), DataManager->UISettings.BattleMenuWidget_WBP);
	if (MenuLayoutWidget == nullptr)
		return;

	HUDLayoutWidget = CreateWidget<USB_BattleHUDWidget>(GetWorld(), DataManager->UISettings.BattleHUDWidget_WBP);
	if (HUDLayoutWidget == nullptr)
		return;

	for (auto& MapRow : DataManager->UISettings.BattleMenuWidgets)
	{
		UUserWidget* const CreatedWidget = MenuLayoutWidget->LoadWidget(MapRow.Value, MapRow.Key);
		if (CreatedWidget)
		{
			MenuWidgets.Add(MapRow.Key, CreatedWidget);
		}
	}
}

void ASB_UIManager::ToggleMenu(bool bNewIsOpen)
{
	if (MenuLayoutWidget == nullptr)
		return;

	if (bNewIsOpen == true)
	{
		if (bIsMenuOpen == false)
		{
			MenuLayoutWidget->AddToViewport();
			bIsMenuOpen = true;
		}
	}
	else
	{
		if (bIsMenuOpen == true)
		{
			MenuLayoutWidget->RemoveFromParent();
			bIsMenuOpen = false;
		}
	}
}

void ASB_UIManager::ToggleHUD(bool bNewIsOpen)
{
	if (HUDLayoutWidget == nullptr)
		return;

	if (bNewIsOpen == true)
	{
		if (bIsHUDOpen == false)
		{
			HUDLayoutWidget->AddToViewport();
			bIsHUDOpen = true;
		}
	}
	else
	{
		if (bIsHUDOpen == true)
		{
			HUDLayoutWidget->RemoveFromParent();
			bIsHUDOpen = false;
		}
	}
}

void ASB_UIManager::OpenMenuWidgetByName(const FName& WidgetName)
{
	if (MenuLayoutWidget && MenuWidgets.FindRef(WidgetName))
	{
		MenuLayoutWidget->SetActiveWidgetByRef(MenuWidgets.FindRef(WidgetName));
	}

	ToggleHUD(false);
	ToggleMenu(true);
}

void ASB_UIManager::UpdateCursor()
{

}

void ASB_UIManager::CreateDamagePopup(float Damage, const AActor* const TargetActor)
{
	if (DataManager == nullptr || HUDLayoutWidget == nullptr)
		return;

	/*URZ_DamageDealtNotificationWidget* const DDNotificationWidget = CreateWidget<URZ_DamageDealtNotificationWidget>(GetWorld(), DataManager->RZ_DamageDealtNotification_WB);
	if (DDNotificationWidget)
	{
		DDNotificationWidget->Init(TargetActor, Damage);
		//HUDLayoutWidget->MainPanel->AddChild(DDNotificationWidget);
	}*/
}

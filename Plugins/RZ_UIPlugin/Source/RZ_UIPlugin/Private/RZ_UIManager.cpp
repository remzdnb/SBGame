// UIPlugin
#include "RZ_UIManager.h"
#include "RZ_UIPluginDataManager.h"
#include "RZ_MenuLayoutWidget.h"
#include "RZ_HUDLayoutWidget.h"
#include "RZ_DamageDealtNotificationWidget.h"
// Engine
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

ARZ_UIManager::ARZ_UIManager() :
	bIsMenuOpen(false),
	bIsHUDOpen(false)
{
}

void ARZ_UIManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARZ_UIPluginDataManager> UIDataManager(GetWorld()); UIDataManager; ++UIDataManager)
	{
		DataManager = *UIDataManager;
		break;
	}

	if (DataManager)
	{
		MenuLayoutWidget = CreateWidget<URZ_MenuLayoutWidget>(GetWorld(), DataManager->RZ_MenuLayout_WB);
		if (MenuLayoutWidget == nullptr)
			return;

		HUDLayoutWidget = CreateWidget<URZ_HUDLayoutWidget>(GetWorld(), DataManager->RZ_HUDLayout_WB);
		if (HUDLayoutWidget == nullptr)
			return;

		for (auto& MapRow : DataManager->MenuWidgets)
		{
			UUserWidget* const CreatedWidget = MenuLayoutWidget->LoadWidget(MapRow.Value, MapRow.Key);
			if (CreatedWidget)
			{
				MenuWidgets.Add(MapRow.Key, CreatedWidget);
			}
		}

		for (auto& MapRow : DataManager->HUDWidgets)
		{
			UUserWidget* const CreatedWidget = HUDLayoutWidget->LoadWidget(MapRow.Value, MapRow.Key);
			if (CreatedWidget)
			{ 
				HUDWidgets.Add(MapRow.Key, CreatedWidget);
			}
		}

		OpenHUD();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_UIManager - ARZ_UIPluginDataManager not found in level."));
	}
}

void ARZ_UIManager::OpenMenu()
{
	if (MenuLayoutWidget == nullptr)
		return;

	if (bIsMenuOpen == false)
	{
		MenuLayoutWidget->AddToViewport();
		bIsMenuOpen = true;

		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerOwner, MenuLayoutWidget);
		PlayerOwner->bShowMouseCursor = true;
	}

	CloseHUD();
}

void ARZ_UIManager::CloseMenu()
{
	if (MenuLayoutWidget == nullptr)
		return;

	if (bIsMenuOpen == true)
	{
		MenuLayoutWidget->RemoveFromParent();
		bIsMenuOpen = false;

		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerOwner);
		PlayerOwner->bShowMouseCursor = false;
	}

	OpenHUD();
}

void ARZ_UIManager::OpenHUD()
{
	if (HUDLayoutWidget == nullptr)
		return;

	if (bIsHUDOpen == false)
	{
		HUDLayoutWidget->AddToViewport();
		bIsHUDOpen = true;
	}
}

void ARZ_UIManager::CloseHUD()
{
	if (HUDLayoutWidget == nullptr)
		return;

	if (bIsHUDOpen == true)
	{
		HUDLayoutWidget->RemoveFromParent();
		bIsHUDOpen = false;
	}
}

void ARZ_UIManager::OpenMenuWidgetByName(const FName& WidgetName)
{
	if (MenuLayoutWidget && MenuWidgets.FindRef(WidgetName))
	{
		MenuLayoutWidget->SetActiveWidgetByRef(MenuWidgets.FindRef(WidgetName));
	}

	CloseHUD();
	OpenMenu();
}

void ARZ_UIManager::OpenHUDWidgetByName(const FName& WidgetName)
{
}

void ARZ_UIManager::UpdateCursor()
{

}

void ARZ_UIManager::CreateDamagePopup(float Damage, const AActor* const TargetActor)
{
	if (DataManager == nullptr || HUDLayoutWidget == nullptr)
		return;

	URZ_DamageDealtNotificationWidget* const DDNotificationWidget = CreateWidget<URZ_DamageDealtNotificationWidget>(GetWorld(), DataManager->RZ_DamageDealtNotification_WB);
	if (DDNotificationWidget)
	{
		DDNotificationWidget->Init(TargetActor, Damage);
		//HUDLayoutWidget->MainPanel->AddChild(DDNotificationWidget);
	}
}

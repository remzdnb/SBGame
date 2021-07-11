//
#include "SB_BattleMenuWidget.h"
#include "SB_UIManager.h"
#include "SB_DataManager.h"
#include "RZ_ButtonWidget.h"
//
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void USB_BattleMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TActorIterator<ASB_DataManager> UIDataManager(GetWorld()); UIDataManager; ++UIDataManager)
	{
		DataManager = *UIDataManager;
		break;
	}

	TopBarHBox->ClearChildren();
	WidgetSwitcher->ClearChildren();
}

class UUserWidget* USB_BattleMenuWidget::LoadWidget(TSubclassOf<UUserWidget> WidgetClass, FName TabName, bool bShouldCreateTab, class URZ_ButtonWidget* TabToAssign)
{
	if (DataManager == nullptr)
		return nullptr;

	UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (CreatedWidget)
	{
		uint8 NewWidgetID = LoadedWidgets.Num(); // Find new valid ID

		// Create / Load Tab

		if (bShouldCreateTab)
		{
			URZ_ButtonWidget* NewTabWidget = CreateWidget<URZ_ButtonWidget>(GetWorld(), DataManager->UISettings.Button_Large_WBP); // Create new TabWidget
			if (NewTabWidget)
			{
				TopBarHBox->AddChild(NewTabWidget);
				TabWidgets.Add(NewTabWidget);
				NewTabWidget->Init(NewWidgetID, TabName);
				NewTabWidget->OnButtonPressed.AddUniqueDynamic(this, &USB_BattleMenuWidget::SetActiveWidgetByIndex);
			}
		}
		else
		{
			TabWidgets.Add(TabToAssign);
			TabToAssign->Init(NewWidgetID, TabName);
			//TabToAssign->OnButtonPressed.AddDynamic(this, &USB_BattleMenuWidget::SetActiveWidget);
		}

		// Load widget

		WidgetSwitcher->AddChild(CreatedWidget);
		LoadedWidgets.Add(CreatedWidget);

		SetActiveWidgetByIndex(0); // Hack

		return CreatedWidget;
	}

	return nullptr;
}

void USB_BattleMenuWidget::SetActiveWidgetByIndex(uint8 Index)
{
	if (Index == SwitcherIndex)
		return;

	/*if (LoadedWidgets.IsValidIndex(SwitcherIndex))
	{
		IRZ_BaseMenuInterface* ClosedWidgetInterface = Cast<IRZ_BaseMenuInterface>(LoadedWidgets[SwitcherIndex]);
		if (ClosedWidgetInterface)
		{
			ClosedWidgetInterface->RemovedFromViewportCallback();
		}
	}*/

	// Set all tabs as unselected, then select the right one.

	for (URZ_ButtonWidget* TabWidget : TabWidgets)
		TabWidget->SetIsSelected_BPN(false);
	TabWidgets[Index]->SetIsSelected_BPN(true);

	// Widget switcher

	WidgetSwitcher->SetActiveWidgetIndex(Index);
	SwitcherIndex = Index;

	//

	/*if (LoadedWidgets.IsValidIndex(SwitcherIndex))
	{
		IRZ_BaseMenuInterface* OpenedWidgetInterface = Cast<IRZ_BaseMenuInterface>(LoadedWidgets[SwitcherIndex]);
		if (OpenedWidgetInterface)
		{
			OpenedWidgetInterface->AddedToViewportCallback();
		}
	}*/
}

void USB_BattleMenuWidget::SetActiveWidgetByRef(UUserWidget* WidgetRef)
{
	if (WidgetSwitcher->GetActiveWidget() == WidgetRef)
		return;

	WidgetSwitcher->SetActiveWidget(WidgetRef);
}
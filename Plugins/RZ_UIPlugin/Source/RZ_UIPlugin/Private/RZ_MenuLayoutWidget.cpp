// RZEssentials Plugin
#include "RZ_MenuLayoutWidget.h"
#include "Input/RZ_ButtonWidget.h"
#include "RZ_UIManager.h"
#include "RZ_UIPluginDataManager.h"
//
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void URZ_MenuLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TActorIterator<ARZ_UIPluginDataManager> UIDataManager(GetWorld()); UIDataManager; ++UIDataManager)
	{
		DataManager = *UIDataManager;
		break;
	}

	TopBarHBox->ClearChildren();
	WidgetSwitcher->ClearChildren();
}

class UUserWidget* URZ_MenuLayoutWidget::LoadWidget(TSubclassOf<UUserWidget> WidgetClass, FName TabName, bool bShouldCreateTab, class URZ_ButtonWidget* TabToAssign)
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
			URZ_ButtonWidget* NewTabWidget = CreateWidget<URZ_ButtonWidget>(GetWorld(), DataManager->RZ_Button_Large_WB); // Create new TabWidget
			if (NewTabWidget)
			{
				TopBarHBox->AddChild(NewTabWidget);
				TabWidgets.Add(NewTabWidget);
				NewTabWidget->Init(NewWidgetID, TabName);
				//NewTabWidget->OnButtonPressed.AddDynamic(this, &URZ_MenuLayoutWidget::SetActiveWidget);

			}
		}
		else
		{
			TabWidgets.Add(TabToAssign);
			TabToAssign->Init(NewWidgetID, TabName);
			//TabToAssign->OnButtonPressed.AddDynamic(this, &URZ_MenuLayoutWidget::SetActiveWidget);
		}

		// Load widget

		WidgetSwitcher->AddChild(CreatedWidget);
		LoadedWidgets.Add(CreatedWidget);

		// Hack

		//SetActiveWidget(0);

		return CreatedWidget;
	}

	return nullptr;
}

/*void URZ_MenuLayoutWidget::SetActiveWidget(uint8 NewTabID)
{
	if (NewTabID == SwitcherIndex)
		return;
	
	//

	if (LoadedWidgets.IsValidIndex(SwitcherIndex))
	{
		IRZ_BaseMenuInterface* ClosedWidgetInterface = Cast<IRZ_BaseMenuInterface>(LoadedWidgets[SwitcherIndex]);
		if (ClosedWidgetInterface)
		{
			ClosedWidgetInterface->RemovedFromViewportCallback();
		}
	}

	// Set all tabs as unselected, then select the right one.

	for (URZ_ButtonWidget* TabWidget : TabWidgets)
		TabWidget->SetIsSelected_BPN(false);
	TabWidgets[NewTabID]->SetIsSelected_BPN(true);

	// Widget switcher

	WidgetSwitcher->SetActiveWidgetIndex(NewTabID);
	SwitcherIndex = NewTabID;

	//

	if (LoadedWidgets.IsValidIndex(SwitcherIndex))
	{
		IRZ_BaseMenuInterface* OpenedWidgetInterface = Cast<IRZ_BaseMenuInterface>(LoadedWidgets[SwitcherIndex]);
		if (OpenedWidgetInterface)
		{
			OpenedWidgetInterface->AddedToViewportCallback();
		}
	}
}*/

void URZ_MenuLayoutWidget::SetActiveWidgetByRef(UUserWidget* WidgetRef)
{
	if (WidgetSwitcher->GetActiveWidget() == WidgetRef)
		return;

	WidgetSwitcher->SetActiveWidget(WidgetRef);
}
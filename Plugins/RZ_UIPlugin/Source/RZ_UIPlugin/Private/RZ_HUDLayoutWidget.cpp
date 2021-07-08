// RZUIPlugin
#include "RZ_HUDLayoutWidget.h"
#include "RZ_CrosshairWidget.h"
#include "RZ_SpectatorWidget.h"
#include "RZ_TargetInfoWidget.h"
#include "RZ_UIPluginDataManager.h"
//
#include "Components/Border.h"
#include "EngineUtils.h"

void URZ_HUDLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TActorIterator<ARZ_UIPluginDataManager> UIDataManager(GetWorld()); UIDataManager; ++UIDataManager)
	{
		DataManager = *UIDataManager;
		break;
	}
}

UUserWidget* URZ_HUDLayoutWidget::LoadWidget(TSubclassOf<UUserWidget> WidgetClass, FName TabName)
{
	UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (CreatedWidget)
	{
		CreatedWidget->AddToViewport();
		return CreatedWidget;
	}

	return nullptr;
}

void URZ_HUDLayoutWidget::SetCrosshairVisibility(bool bNewIsVisible)
{
	if (CrosshairWidget)
	{
		if (bNewIsVisible)
			CrosshairWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		else
			CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

///// RZ_HUDLayoutWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_HUDLayoutWidget.generated.h"

UCLASS()
class RZ_UIPLUGIN_API URZ_HUDLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION() class UUserWidget* LoadWidget(TSubclassOf<UUserWidget> WidgetClass, FName TabName);
	UFUNCTION() void SetCrosshairVisibility(bool bIsVisible);

private:
	
	class ARZ_UIPluginDataManager* DataManager;

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) class URZ_CrosshairWidget* CrosshairWidget;
};
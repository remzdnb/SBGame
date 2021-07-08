#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_MenuLayoutWidget.generated.h"

UCLASS()
class RZ_UIPLUGIN_API URZ_MenuLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UFUNCTION() // Load UserWidget into the widget switcher and create associated top bar tab.
	class UUserWidget* LoadWidget(TSubclassOf<UUserWidget> WidgetClass, FName TabName, bool bShouldCreateTab = true, class URZ_ButtonWidget* TabToAssign = nullptr);

	/*UFUNCTION() //
	void SetActiveWidget(uint8 NewWidgetID);*/

	UFUNCTION()
	void SetActiveWidgetByRef(UUserWidget* WidgetRef);

private:

	class ARZ_UIPluginDataManager* DataManager;

	UPROPERTY() TArray<class URZ_ButtonWidget*> TabWidgets;
	UPROPERTY() TArray<UUserWidget*> LoadedWidgets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 SwitcherIndex = -1;

#pragma region +++++ UMG Bindings ...

private:

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* TopBarHBox;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class URZ_ButtonWidget* ExitTab;

#pragma endregion
	
};

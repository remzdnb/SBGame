#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipConfigWidget.generated.h"

class UPanelWidget;
class UTextBlock;
class UImage;
class UWidgetSwitcher;
//
class USB_VehicleSelectionItemWidget;
class USB_SlotSelectionItemWidget;
class USB_ModuleSelectionItemWidget;
class URZ_ButtonWidget;
class URZ_StatWidget;

UCLASS()
class USB_ShipConfigWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

private:
	
	class USB_GameInstance* GInstance;
	class ASB_CampaignPlayerController* OwningPC;
	TWeakObjectPtr<class ASB_Ship> OwnedShip;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Main

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnVehicleSelectionButtonPressedBPN(uint8 ButtonID);

	UFUNCTION(BlueprintNativeEvent)
	void OnSlotSelectionButtonPressedBPN(uint8 ButtonID);

private:

	UFUNCTION()
	void OnNewOwnedShip(class ASB_Ship* const NewOwnedShip);

	UFUNCTION()
	void OnNewMenuWidgetSelected(const FName& WidgetName, UUserWidget* SelectedWidget);

	//

	UPROPERTY(meta = (BindWidget)) UPanelWidget* MainCanvas;
	UPROPERTY(meta = (BindWidget)) URZ_ButtonWidget* VehicleSelectionButton;
	UPROPERTY(meta = (BindWidget)) URZ_ButtonWidget* SlotSelectionButton;
	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* VehicleToSlotSwitcher;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Vehicle selection panel

private:
	
	UFUNCTION()
	void UpdateVehicleSelectionPanel();

	UFUNCTION()
	void OnVehicleSelectionItemPressed(const FName& VehicleDataRowName);

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USB_VehicleSelectionItemWidget> VehicleSelectionItemWBP;
	
	UPROPERTY(meta = (BindWidget)) UPanelWidget* VehicleSelectionContainerPanel;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SelectedVehicleNameText;
	UPROPERTY(meta = (BindWidget)) UImage* SelectedVehicleImage;
	UPROPERTY(meta = (BindWidget)) URZ_StatWidget* SelectedVehicleDurabilityStat;
	UPROPERTY(meta = (BindWidget)) URZ_StatWidget* SelectedVehicleMoveSpeedStat;
	UPROPERTY(meta = (BindWidget)) URZ_StatWidget* SelectedVehicleTurnSpeedStat;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Slot selection panel

private:

	UFUNCTION()
	void UpdateSlotSelectionPanel();

	UFUNCTION()
	void OnSlotSelectionItemPressed(uint8 SlotID);

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USB_SlotSelectionItemWidget> SlotSelectionItemWBP;

	UPROPERTY()
	class USB_ModuleSlotComponent* SelectedSlot;
	
	//
	
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* SlotListContainerPanel;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShipNameText;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Module selection panel

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnModuleListOpenedBPI();

private:
	
	UFUNCTION()
	void UpdateModuleSelectionPanel();

	UFUNCTION()
	void OnModuleSelectionItemPressed(const FName& ModuleDataRowName);

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USB_ModuleSelectionItemWidget> ModuleSelectionItemWBP;

	//

	UPROPERTY(meta = (BindWidget)) UPanelWidget* ModuleListMainPanel;
	UPROPERTY(meta = (BindWidget)) UPanelWidget* ModuleListContainerPanel;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ShopSlotTypeText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ShopModuleNameText;
	UPROPERTY(meta = (BindWidget)) UImage* ShopModuleImage;
	UPROPERTY(meta = (BindWidget)) URZ_StatWidget* ShopDurabilityStat;
	UPROPERTY(meta = (BindWidget)) URZ_StatWidget* ShopArmorStat;
};



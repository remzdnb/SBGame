#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipConfigWidget.generated.h"

UCLASS()
class USB_ShipConfigWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
	void OnNewOwnedShip(class ASB_Ship* const NewOwnedShip);

	//

	class ASB_DataManager* DataManager;
	class USB_GameInstance* GInstance;
	class ASB_CampaignPlayerController* OwningPC;
	
	TWeakObjectPtr<class ASB_Ship> OwnedShip;

	FRotator DefaultArmRotation;
	float DefaultArmLength;
	FRotator SelectedArmRotation;
	float SelectedArmLength;

	//

	UPROPERTY(meta = (BindWidget)) class UCanvasPanel* MainCanvas;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Ship Selection

private:
	
	UFUNCTION()
	void OnPrevShipButtonPressed();

	UFUNCTION()
	void OnNextShipButtonPressed();

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShipNameText;
	UPROPERTY(meta = (BindWidget)) class UButton* PrevShipButton;
	UPROPERTY(meta = (BindWidget)) class UButton* NextShipButton;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Slot list : Lists all available slots of currently selected ship.

private:

	UFUNCTION()
	void UpdateSlotList();

	UFUNCTION()
	void OnSlotListItemPressed(uint8 SlotID);

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ModuleWidget> SlotListItemWBP;

	UPROPERTY()
	class USB_ModuleSlotComponent* SelectedSlot;
	
	//

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* SlotListContainerPanel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Module info : Infos about currently selected module.
	///// Module list : Lists all available modules of currently selected slot.

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnModuleListOpenedBPI();

private:
	
	UFUNCTION()
	void UpdateModuleList();

	UFUNCTION()
	void OnModuleListItemPressed(const FName& ModuleDataRowName);

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ModuleListItemWidget> ModuleListItemWBP;

	//

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleListMainPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleListContainerPanel;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShopSlotTypeText;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShopModuleNameText;
	UPROPERTY(meta = (BindWidget)) class UImage* ShopModuleImage;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ShopDurabilityStat;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ShopArmorStat;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Slots - Deprecated

private:

	UFUNCTION()
	void UpdateSlots();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ShipConfig_Slot_Widget> ShipConfig_Slot_WBP;

	//
	
	UPROPERTY(meta = (BindWidget)) class UCanvasPanel* SlotsContainerCanvas;
};



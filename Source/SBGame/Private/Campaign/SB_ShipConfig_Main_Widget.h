#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipConfig_Main_Widget.generated.h"

UCLASS()
class USB_ShipConfig_Main_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnModuleSelectedBPN(const class USB_BaseModule* const NewSelectedModule);

private:

	UFUNCTION()
	void OnNewOwnedShip(class ASB_Ship* const NewOwnedShip);

	UFUNCTION()
	void OnModuleHovered(const class USB_BaseModule* const NewHoveredModule);

	UFUNCTION()
	void OnModuleReplaced();

	UFUNCTION()
	void Debug(float DeltaTime);

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

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Ship selection & Module list

private:

	UFUNCTION()
	void OnPrevShipButtonPressed();

	UFUNCTION()
	void OnNextShipButtonPressed();
	
	UFUNCTION()
	void UpdateModuleList();

	UFUNCTION()
	void OnConfigListModulePressed(uint8 ModuleID, const FName& DataRowName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ModuleWidget> ConfigModuleWBP;
	
	//

	UPROPERTY(meta = (BindWidget)) class UButton* PrevShipButton;
	UPROPERTY(meta = (BindWidget)) class UButton* NextShipButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShipNameText;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleListContainerPanel;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Slots

private:

	UFUNCTION()
	void UpdateSlots();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ShipConfig_Slot_Widget> ShipConfig_Slot_WBP;

	//
	
	UPROPERTY(meta = (BindWidget)) class UCanvasPanel* SlotsContainerCanvas;

public:
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Shop

private:
	
	UFUNCTION()
	void UpdateShop();

	UFUNCTION()
	void OnShopModuleSelected(uint8 NewModuleID, const FName& DataRowName);
	
	UFUNCTION()
	void OnShopApplyButtonPressed(uint8 ButtonID);

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ModuleWidget> ShopModuleWBP;

	//

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ShopMainPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ShopContainerPanel;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShopSlotTypeText;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShopModuleNameText;
	UPROPERTY(meta = (BindWidget)) class UImage* ShopModuleImage;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ShopDurabilityStat;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ShopArmorStat;
	UPROPERTY(meta = (BindWidget)) class URZ_ButtonWidget* ShopApplyButton;


};



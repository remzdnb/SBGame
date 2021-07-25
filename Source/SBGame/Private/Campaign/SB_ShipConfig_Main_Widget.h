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

private:

	UFUNCTION()
	void OnNewOwnedShip(class ASB_Ship* const NewOwnedShip);

	UFUNCTION()
	void OnModuleHovered(const class USB_BaseModule* const NewHoveredModule);

	UFUNCTION()
	void OnModuleSelected(const class USB_BaseModule* const NewSelectedModule);

	UFUNCTION()
	void OnModuleReplaced();
	
	//

	UFUNCTION()
	void RefreshSlotWidgets();

	UFUNCTION()
	void RefreshModuleWidgets();

	UFUNCTION()
	void RefreshModuleListWidgets();

	UFUNCTION()
	void Debug(float DeltaTime);

	//

	class ASB_DataManager* DataManager;
	class ASB_CampaignPlayerController* OwningPC;
	
	TWeakObjectPtr<class ASB_Ship> OwnedShip;

	//TArray<class USB_ShipConfig_Slot_Widget*> SlotWidgets;
	TArray<UUserWidget*> ModuleWidgets;

	FRotator DefaultArmRotation;
	float DefaultArmLength;
	FRotator SelectedArmRotation;
	float SelectedArmLength;

	//

	UPROPERTY(meta = (BindWidget)) class UCanvasPanel* MainCanvas;
	UPROPERTY(meta = (BindWidget)) class UCanvasPanel* SlotsContainerCanvas;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleSelectionMainPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleSelectionContainerPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleListContainerPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ShipConfig_Slot_Widget> ShipConfig_Slot_WBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ShipConfig_Module_Widget> ShipConfig_Module_WBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USB_ModuleWidget> ModuleSlotWBP;
};



#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ModuleWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressedDelegate, uint8, ModuleID, const FName&, ModuleRowName);

UCLASS()
class USB_ModuleWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	USB_ModuleWidget(const FObjectInitializer& ObjectInitializer);

protected:
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	void Update(const class ASB_DataManager* const NewDataManager, const FSB_ModuleSlotData& NewSlotData, const FName& NewModuleDataRowName);
	
	/*void Update(
		class USB_BaseModule* const ModuleRef,
		uint8 NewModuleID = 0,
		const FName& NewDataRowName = "Default",
		const FSB_BaseModuleData* const ModuleData = nullptr);*/

	//

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectionUpdatedBPI(bool bNewIsSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateUpdatedBPI(ESB_ModuleState NewState);

	UFUNCTION()
	void OnDurabilityUpdated(float NewDurability);

	//

	FPressedDelegate OnPressed;

private:

	const class USB_GameInstance* GInstance;
	
	TWeakObjectPtr<class USB_BaseModule> BaseModuleRef;

	class ASB_DataManager* DataManager;

	class UDataTable* BaseModuleDT;

	//

	bool bIsHovered;
	bool bIsPressed;

	uint8 ModuleID;
	FName DataRowName;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotID;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotName;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleName;
	UPROPERTY(meta = (BindWidget)) class UImage* ModuleImage;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleStatusProgressBar;
};

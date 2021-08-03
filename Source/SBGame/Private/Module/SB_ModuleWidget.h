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

protected:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	void Update(
		class USB_BaseModule* const ModuleRef,
		uint8 NewModuleID = 0,
		const FName& NewDataRowName = "Default",
		const FSB_BaseModuleData* const ModuleData = nullptr);

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
	
	TWeakObjectPtr<USB_BaseModule> BaseModuleRef;

	//

	bool bIsHovered;
	bool bIsPressed;

	uint8 ModuleID;
	FName DataRowName;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleSlotIDText;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleSlotTypeText;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleRowNameText;
	UPROPERTY(meta = (BindWidget)) class UImage* ModuleImage;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleStatusProgressBar;
};
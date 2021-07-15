#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ModuleWidget.generated.h"

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

	UFUNCTION()
	void Init(const class ASB_DataManager* const NewDataManager, class ASB_Ship* const NewShipRef, class USB_BaseModule* const NewBaseModuleRef);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectionUpdatedBPI(bool bNewIsSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateUpdatedBPI(ESB_ModuleState NewState);

	UFUNCTION()
	void OnDurabilityUpdated(float NewDurability);

private:

	const class ASB_DataManager* DataManager;

	//

	UPROPERTY() class ASB_Ship* ShipRef;
	UPROPERTY() class USB_BaseModule* BaseModuleRef;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleSlotText;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleClassText;
	UPROPERTY(meta = (BindWidget)) class UImage* ModuleImage;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleStatusProgressBar;
};

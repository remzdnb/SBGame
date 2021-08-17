#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_SlotSelectionItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotSelectionItemPressedDelegate, uint8, SlotID);

UCLASS()
class USB_SlotSelectionItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	USB_SlotSelectionItemWidget(const FObjectInitializer& ObjectInitializer);

protected:
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void Update(const FSB_ModuleSlotData& NewSlotData, const class USB_BaseModule* const NewModuleRef);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectionUpdatedBPI(bool bNewIsSelected);

	//

	FSlotSelectionItemPressedDelegate OnSlotSelectionItemPressed;

private:

	const class USB_GameInstance* GInstance;

	FSB_ModuleSlotData SlotData;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotID;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotName;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleName;
};

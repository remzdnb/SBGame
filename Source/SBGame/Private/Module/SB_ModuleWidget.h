#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ModuleWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressedDelegate, uint8, SlotID);

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

	UFUNCTION()
	void Update(const FSB_ModuleSlotData& NewSlotData, const class USB_BaseModule* const NewModuleRef);

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

	FSB_ModuleSlotData SlotData;
	TWeakObjectPtr<const class USB_BaseModule> ModuleRef;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotID;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotName;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleName;
	UPROPERTY(meta = (BindWidget)) class UImage* ModuleImage;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ModuleStatusProgressBar;
};

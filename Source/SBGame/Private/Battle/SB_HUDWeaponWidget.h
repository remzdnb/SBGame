#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_HUDWeaponWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHUDWeaponWidgetPressedDelegate, uint8, SlotID);

UCLASS()
class USB_HUDWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	USB_HUDWeaponWidget(const FObjectInitializer& ObjectInitializer);

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

	//

	FHUDWeaponWidgetPressedDelegate OnHUDWeaponWidgetPressed;

private:

	const class USB_GameInstance* GInstance;

	FSB_ModuleSlotData SlotData;
	TWeakObjectPtr<const class USB_BaseModule> ModuleRef;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* SlotID;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* WeaponName;
	UPROPERTY(meta = (BindWidget)) class UImage* WeaponImage;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* WeaponStatusPBar;
};

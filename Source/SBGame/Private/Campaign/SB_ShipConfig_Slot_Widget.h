#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipConfig_Slot_Widget.generated.h"

UCLASS()
class USB_ShipConfig_Slot_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void Init(class ASB_Ship* const NewShipRef, class USB_BaseModule* const NewModuleRef);

	//
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:

	TWeakObjectPtr<class ASB_Ship> ShipRef;
	TWeakObjectPtr<class USB_BaseModule> ModuleRef;

	//

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* InfoPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* TargetPanel;
	UPROPERTY(meta = (BindWidget)) class UImage* TargetImage;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleName;
	
};

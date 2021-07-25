#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipConfig_Module_Widget.generated.h"

UCLASS()
class USB_ShipConfig_Module_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void Init(class ASB_DataManager* const NewDataManager, class ASB_Ship* const NewShipRef, uint8 NewModuleID, const FName& NewDataRowName);

private:

	class ASB_DataManager* DataManager;

	TWeakObjectPtr<ASB_Ship> ShipRef;

	uint8 ModuleID;
	FName DataRowName;

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleNameText;
	UPROPERTY(meta = (BindWidget)) class UImage* ModuleThumbnailImage;
};


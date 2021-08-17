#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ModuleSelectionItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModuleSelectionItemPressedDelegate, const FName&, ModuleName);

UCLASS()
class USB_ModuleSelectionItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void Update(const FName& NewModuleDataRowName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectionUpdatedBPI(bool bNewIsSelected);

	//

	FModuleSelectionItemPressedDelegate OnModuleSelectionItemPressed;

private:

	FName ModuleDataRowName;
	
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleNameText;
	UPROPERTY(meta = (BindWidget)) class UImage* ModuleImage;
};

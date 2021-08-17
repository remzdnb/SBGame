#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_VehicleSelectionItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVehicleSelectionItemPressed, const FName&, VehicleDataRowName);

UCLASS()
class USB_VehicleSelectionItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void Update(const FName& NewVehicleDataRowName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectionUpdatedBPI(bool bNewIsSelected);

	//

	FVehicleSelectionItemPressed OnVehicleSelectionItemPressed;

private:

	FName VehicleDataRowName;
	
	UPROPERTY(meta = (BindWidget)) class UTextBlock* VehicleNameText;
	UPROPERTY(meta = (BindWidget)) class UImage* VehicleImage;
};

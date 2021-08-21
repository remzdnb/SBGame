#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_HUDVehicleOTMWidget.generated.h"

#define OTM_ZOFFSET 10000.0f

UCLASS()
class USB_HUDVehicleOTMWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void Init(class ASB_Vehicle* const NewOwnedVehicle);

	UFUNCTION()
	void OnVehicleDurabilityUpdated(float NewDurability, float MaxDurability);

	UFUNCTION()
	void OnShieldDurabilityUpdated(float NewDurability, float MaxDurability);

private:

	const class ASB_DataManager* DataManager;

	TWeakObjectPtr<ASB_Vehicle> OwnedVehicle;

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget)) class URZ_ProgressBarWidget* VehicleDurabilityPBar;
	UPROPERTY(meta = (BindWidget)) class URZ_ProgressBarWidget* ShieldDurabilityPBar;
};

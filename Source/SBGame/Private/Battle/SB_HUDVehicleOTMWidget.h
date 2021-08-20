#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_HUDVehicleOTMWidget.generated.h"

#define OTM_ZOFFSET 7500.0f

UCLASS()
class USB_HUDVehicleOTMWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void Init(class ASB_Ship* const NewOwnedShip);

	UFUNCTION()
	void OnShipDurabilityUpdated(float NewDurability, float MaxDurability);

	UFUNCTION()
	void OnShieldDurabilityUpdated(float NewDurability, float MaxDurability);

private:

	const class ASB_DataManager* DataManager;

	TWeakObjectPtr<ASB_Ship> OwnedShip;

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget)) class URZ_ProgressBarWidget* ShipDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) class URZ_ProgressBarWidget* ShieldDurabilityProgressBar;
};

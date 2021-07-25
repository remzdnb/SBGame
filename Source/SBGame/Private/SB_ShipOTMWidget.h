///// SB_ShipOTMWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipOTMWidget.generated.h"

#define OTM_ZOFFSET 7500.0f

UCLASS()
class USB_ShipOTMWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void Init(const class ASB_DataManager* const NewDataManager, class ASB_Ship* const NewOwnedShip);

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

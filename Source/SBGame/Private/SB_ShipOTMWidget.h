///// SB_ShipOTMWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ShipOTMWidget.generated.h"

#define OTM_ZOFFSET 5000.0f

class ASB_DataManager;
class ASB_Ship;
class URZ_ProgressBarWidget;
class UPanelWidget;
class UTextBlock;

UCLASS()
class USB_ShipOTMWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() void Init(const ASB_DataManager* const NewDataManager, ASB_Ship* const NewShipRef);
	UFUNCTION() void Update(float NewDurability);

private:

	const ASB_DataManager* DataManager;

	TWeakObjectPtr<ASB_Ship> ShipRef;

	UPROPERTY(meta = (BindWidget)) UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* DurabilityProgressBar;
};

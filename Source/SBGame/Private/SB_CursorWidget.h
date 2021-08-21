#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_CursorWidget.generated.h"

UCLASS()
class USB_CursorWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnNewOwnedVehicle(class ASB_Vehicle* const NewOwnedVehicle);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayStartAnimationBPI(bool bShouldStart);

private:

	UFUNCTION()
	void OnHoveredModuleUpdated(const class USB_BaseModule* const NewHoveredModule);

	UFUNCTION()
	void OnSelectedModuleUpdated(const class USB_BaseModule* const NewSelectedModule);
	
	//

	class ASB_CampaignPlayerController* CampaignPC;

	TWeakObjectPtr<class ASB_Vehicle> OwnedVehicle;
	
	//
	
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* CrosshairPanel;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ModuleInfoPanel;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ModuleInfoNameText;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ModuleTypeStat;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ModuleInfoDurabilityStat;
	UPROPERTY(meta = (BindWidget)) class URZ_StatWidget* ModuleInfoArmorStat;
};

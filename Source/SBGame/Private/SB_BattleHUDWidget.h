///// SB_BattleHUDWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_BattleHUDWidget.generated.h"

class ASB_DataManager;
class USB_ModuleWidget;
class UCanvasPanel;
class UPanelWidget;


UCLASS()
class USB_BattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void OnNewOwnedShip(class ASB_Ship* const NewOwnedShip);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShipMovementUpdatedBPI(int32 ForwardInput, int32 RightInput);

	UFUNCTION()
	void OnShipDurabilityUpdated(float NewDurability);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShipSpawnedBPI();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShipDestroyedBPI(const APlayerState* const InstigatorPS);

	//

	FORCEINLINE UFUNCTION() UCanvasPanel* const GetMainCanvas() const { return MainCanvas; }

private:

	const ASB_DataManager* DataManager;
	class ASB_PlayerController* OwnerPC; //remove

	//

	UPROPERTY(meta = (BindWidget)) UCanvasPanel* MainCanvas;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* ShipDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) UPanelWidget* WeaponModulesContainer;
	UPROPERTY(meta = (BindWidget)) UPanelWidget* ThrusterModulesContainer;
	UPROPERTY(meta = (BindWidget)) USB_ModuleWidget* ShieldModuleWidget;
};

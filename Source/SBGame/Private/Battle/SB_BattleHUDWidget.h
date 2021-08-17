///// SB_BattleHUDWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_BattleHUDWidget.generated.h"

class USB_GameInstance;
//
class UCanvasPanel;
class UPanelWidget;
class UProgressBar;
class USB_ModuleWidget;
class URZ_ProgressBarWidget;

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
	void OnShipDurabilityUpdated(float NewDurability, float MaxDurability);

	UFUNCTION()
	void OnShieldDurabilityUpdated(float NewDurability, float MaxDurability);

	UFUNCTION()
	void OnShieldCooldownUpdated(float RemainingTime, float MaxTime);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShipSpawnedBPI();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShipDestroyedBPI(const APlayerState* const InstigatorPS);

	//

	FORCEINLINE UFUNCTION() UCanvasPanel* const GetMainCanvas() const { return MainCanvas; }

private:

	USB_GameInstance* GInstance;
	class ASB_PlayerController* OwnerPC; //remove

	//

	UPROPERTY(meta = (BindWidget)) UCanvasPanel* MainCanvas;
	UPROPERTY(meta = (BindWidget)) UPanelWidget* WeaponModulesContainer;
	UPROPERTY(meta = (BindWidget)) UPanelWidget* ThrusterModulesContainer;
	UPROPERTY(meta = (BindWidget)) USB_ModuleWidget* ShieldModuleWidget;
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* ShipDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* ShieldDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* ShieldCooldownProgressBar;
};
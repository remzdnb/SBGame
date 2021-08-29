///// SB_HUDMainWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SB_Types.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_HUDMainWidget.generated.h"

class USB_GameInstance;
class USB_HUDWeaponWidget;
//
class UCanvasPanel;
class UPanelWidget;
class UProgressBar;
class USB_ModuleWidget;
class URZ_ProgressBarWidget;

UCLASS()
class USB_HUDMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void OnNewOwnedVehicle(class ASB_Vehicle* const NewOwnedVehicle);

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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Vehicle

public:

	UFUNCTION()
	void OnVehicleDurabilityUpdated(float NewDurability, float MaxDurability);

	UFUNCTION()
	void OnShieldDurabilityUpdated(float NewDurability, float MaxDurability);

	UFUNCTION()
	void OnShieldCooldownUpdated(float RemainingTime, float MaxTime);

private:
	
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* ShipDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* ShieldDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) URZ_ProgressBarWidget* ShieldCooldownProgressBar;
	
	//UPROPERTY(meta = (BindWidget)) USB_ModuleWidget* ShieldModuleWidget;
	//UPROPERTY(meta = (BindWidget)) UPanelWidget* ThrusterModulesContainer;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Weapons

private:
	
	UPROPERTY(meta = (BindWidget)) UPanelWidget* WeaponModulesContainer;
};
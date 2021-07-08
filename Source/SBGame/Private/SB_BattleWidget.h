#pragma once

#include "SB_Types.h"
//
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_BattleWidget.generated.h"


UCLASS()
class USB_BattleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

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

private:

	class ASB_PlayerController* OwnerPC;
	class ASB_DataManager* DataManager;

	//

	UPROPERTY(meta = (BindWidget)) class UProgressBar* ShipDurabilityProgressBar;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* WeaponModulesContainer;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ThrusterModulesContainer;
	UPROPERTY(meta = (BindWidget)) class USB_ModuleWidget* ShieldModuleWidget;
};

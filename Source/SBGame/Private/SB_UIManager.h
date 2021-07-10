///// SB_UIManager.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SB_UIManager.generated.h"

UCLASS()
class ASB_UIManager : public AHUD
{
	GENERATED_BODY()

public:

	ASB_UIManager();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//

	UPROPERTY() class USB_BattleMenuWidget* MenuLayoutWidget;
	UPROPERTY() class USB_BattleHUDWidget* HUDLayoutWidget;

	//

	UFUNCTION(BlueprintCallable) void ToggleMenu(bool bNewIsOpen);
	UFUNCTION(BlueprintCallable) void ToggleHUD(bool bNewIsOpen);
	UFUNCTION(BlueprintCallable) void OpenMenuWidgetByName(const FName& WidgetName);

	UFUNCTION(BlueprintCallable) void UpdateCursor();
	UFUNCTION(BlueprintCallable) void CreateDamagePopup(float Damage, const class AActor* const TargetActor);

	UFUNCTION() FORCEINLINE bool IsMenuOpen() const { return bIsMenuOpen; }
	UFUNCTION() FORCEINLINE bool IsHUDOpen() const { return bIsHUDOpen; }
	UFUNCTION() FORCEINLINE class UUserWidget* GetMenuWidgetByName(const FName& WidgetName) const { return MenuWidgets.FindRef(WidgetName); }

private:

	class ASB_DataManager* DataManager;

	TMap<FName, class UUserWidget*> MenuWidgets;

	bool bIsMenuOpen;
	bool bIsHUDOpen;
};

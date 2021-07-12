///// SB_UIManager.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SB_UIManager.generated.h"

class ASB_DataManager;
class USB_BattleMenuWidget;
class USB_BattleHUDWidget;

UCLASS()
class ASB_UIManager : public AHUD
{
	GENERATED_BODY()

public:

	ASB_UIManager();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//

	UFUNCTION() void ToggleMenu(bool bNewIsOpen);
	UFUNCTION() void ToggleHUD(bool bNewIsOpen);

	UFUNCTION() void AddHUDWidget(UUserWidget* NewWidget);
	UFUNCTION() void OpenMenuWidgetByName(const FName& WidgetName);

	UFUNCTION() void UpdateCursor();
	UFUNCTION() void CreateDamagePopup(float Damage, const AActor* const TargetActor);

	//

	UFUNCTION() FORCEINLINE bool IsMenuOpen() const { return bIsMenuOpen; }
	UFUNCTION() FORCEINLINE bool IsHUDOpen() const { return bIsHUDOpen; }
	UFUNCTION() FORCEINLINE UUserWidget* GetMenuWidgetByName(const FName& WidgetName) const { return MenuWidgets.FindRef(WidgetName); }

private:

	const ASB_DataManager* DataManager;

	USB_BattleMenuWidget* BattleMenuWidget;
	USB_BattleHUDWidget* BattleHUDWidget;

	TMap<FName, UUserWidget*> MenuWidgets;

	bool bIsMenuOpen;
	bool bIsHUDOpen;
};

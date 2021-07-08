///// ARZ_UIManager - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RZ_UIManager.generated.h"

UCLASS()
class RZ_UIPLUGIN_API ARZ_UIManager : public AHUD
{
	GENERATED_BODY()

public:

	ARZ_UIManager();

///// AActor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;

///// ARZ_UIManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY() class URZ_MenuLayoutWidget* MenuLayoutWidget;
	UPROPERTY() class URZ_HUDLayoutWidget* HUDLayoutWidget;

	//

	UFUNCTION(BlueprintCallable) void OpenMenu();
	UFUNCTION(BlueprintCallable) void CloseMenu();
	UFUNCTION(BlueprintCallable) void OpenHUD();
	UFUNCTION(BlueprintCallable) void CloseHUD();
	UFUNCTION(BlueprintCallable) void OpenMenuWidgetByName(const FName& WidgetName);
	UFUNCTION(BlueprintCallable) void OpenHUDWidgetByName(const FName& WidgetName);

	UFUNCTION() FORCEINLINE bool IsMenuOpen() const { return bIsMenuOpen; }
	UFUNCTION() FORCEINLINE bool IsHUDOpen() const { return bIsHUDOpen; }
	UFUNCTION() FORCEINLINE class UUserWidget* GetMenuWidgetByName(const FName& WidgetName) const { return MenuWidgets.FindRef(WidgetName); }
	UFUNCTION() FORCEINLINE class UUserWidget* GetHUDWidgetByName(const FName& WidgetName) const { return HUDWidgets.FindRef(WidgetName); }

	//

	UFUNCTION(BlueprintCallable) void UpdateCursor();
	UFUNCTION(BlueprintCallable) void CreateDamagePopup(float Damage, const class AActor* const TargetActor);

protected:

	UPROPERTY() TMap<FName, class UUserWidget*> MenuWidgets;
	UPROPERTY() TMap<FName, class UUserWidget*> HUDWidgets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsMenuOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHUDOpen;

private:

	UPROPERTY() class ARZ_UIPluginDataManager* DataManager;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RZ_UIManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNewMenuWidgetSelectedDelegate, const FName&, WidgetName, UUserWidget*, SelectedWidget);

UCLASS()
class RZ_UIPLUGIN_API ARZ_UIManager : public AHUD
{
	GENERATED_BODY()

public:

	ARZ_UIManager();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//

	UFUNCTION() void ToggleMenu(bool bNewIsOpen);
	UFUNCTION() void ToggleHUD(bool bNewIsOpen);

	UFUNCTION() void CreateMenuWidget(const FName& WidgetName, TSubclassOf<UUserWidget> WidgetClass);
	
	UFUNCTION() void AddHUDWidget(UUserWidget* NewWidget);

	UFUNCTION() void BroadcastMenuWidgetSelected(const FName& WidgetName, UUserWidget* WidgetRef);

	//

	UFUNCTION() FORCEINLINE bool IsMenuOpen() const { return bIsMenuOpen; }
	UFUNCTION() FORCEINLINE bool IsHUDOpen() const { return bIsHUDOpen; }

	//

	FNewMenuWidgetSelectedDelegate OnNewMenuWidgetSelected;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MenuLayoutWBP;

	//

	class URZ_MenuLayoutWidget* MenuLayoutWidget;
	
	bool bIsMenuOpen;
	bool bIsHUDOpen;
};
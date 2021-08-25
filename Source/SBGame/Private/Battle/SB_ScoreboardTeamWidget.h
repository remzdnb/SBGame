#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ScoreboardTeamWidget.generated.h"

class USB_GameInstance;
class ASB_GameState;
class UPanelWidget;
class UBorder;
class UTextBlock;

UCLASS()
class USB_ScoreboardTeamWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	//
	
	UFUNCTION()
	void Init(uint8 NewTeamID);

	UFUNCTION()
	void Update();

private:

	USB_GameInstance* GInstance;
	ASB_GameState* GState;
	
	//
	
	UPROPERTY()
	uint8 TeamID;
	
	//

	UPROPERTY(meta = (BindWidget)) UBorder* TeamBorder;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TeamNameText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlayerCountText;
	UPROPERTY(meta = (BindWidget)) UPanelWidget* PlayerWidgetsContainerPanel;
};

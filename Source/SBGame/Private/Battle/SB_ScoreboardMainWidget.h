#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ScoreboardMainWidget.generated.h"

class USB_GameInstance;
class ASB_GameState;
class UPanelWidget;

UCLASS()
class USB_ScoreboardMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

private:
	
	UFUNCTION()
	void Update();

	//

	USB_GameInstance* GInstance;
	ASB_GameState* GState;
	
	//

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* TeamsContainerPanel;
};

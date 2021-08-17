#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ScoreboardMainWidget.generated.h"

UCLASS()
class USB_ScoreboardMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

private:
	
	class ASB_GameState* GameState;

	//

	UFUNCTION()
	void Update(bool bDummy);

	//

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* SpectatorTeamContainer;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ATeamContainer;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* BTeamContainer;

};

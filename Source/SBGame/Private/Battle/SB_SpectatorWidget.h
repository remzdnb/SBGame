#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_SpectatorWidget.generated.h"

UCLASS()
class USB_SpectatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void SetIsVisibleBPI(bool bNewIsVisible);
};

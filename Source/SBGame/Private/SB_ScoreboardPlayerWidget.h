#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SB_ScoreboardPlayerWidget.generated.h"

UCLASS()
class USB_ScoreboardPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(const class ASB_PlayerState* const PlayerState);

private:

	UPROPERTY(meta = (BindWidget))	class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))	class UTextBlock* TeamText;
	UPROPERTY(meta = (BindWidget))	class UTextBlock* KillCountText;
	UPROPERTY(meta = (BindWidget))	class UTextBlock* DeathCountText;
};

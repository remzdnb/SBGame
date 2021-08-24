#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SB_GameMode.generated.h"

class USB_GameInstance;
class ASB_GameState;

UCLASS()
class ASB_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASB_GameMode();

	virtual void PostInitializeComponents() override;

protected:

	USB_GameInstance* GInstance;
	ASB_GameState* GState;
};

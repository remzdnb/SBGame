#include "SB_GameMode.h"
#include "SB_GameInstance.h"
#include "SB_GameState.h"

ASB_GameMode::ASB_GameMode()
{
}

void ASB_GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GInstance = Cast<USB_GameInstance>(GetGameInstance());
	GState = Cast<ASB_GameState>(GetWorld()->GetGameState());
}
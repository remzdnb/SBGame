#include "Campaign/SB_CampaignGameMode.h"
#include "Campaign/SB_CampaignPlayerController.h"
#include "RZ_UIManager.h"

ASB_CampaignGameMode::ASB_CampaignGameMode()
{
	PlayerControllerClass = ASB_CampaignPlayerController::StaticClass();
	HUDClass = ARZ_UIManager::StaticClass();
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;
	
	PrimaryActorTick.bCanEverTick = false;
}
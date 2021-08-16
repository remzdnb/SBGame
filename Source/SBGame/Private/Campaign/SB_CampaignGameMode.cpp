#include "Campaign/SB_CampaignGameMode.h"
#include "Campaign/SB_CampaignPlayerController.h"

ASB_CampaignGameMode::ASB_CampaignGameMode()
{
	PlayerControllerClass = ASB_CampaignPlayerController::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}
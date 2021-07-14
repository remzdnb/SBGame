#include "SB_GameInstance.h"
#include "SB_DataManager.h"
//
#include "EngineUtils.h"

void USB_GameInstance::StartGameInstance()
{
	Super::StartGameInstance();

	for (TActorIterator<ASB_DataManager> NewDataManager(GetWorld()); NewDataManager; ++NewDataManager)
	{
		DataManager = *NewDataManager;
		break;
	}
}

void USB_GameInstance::OnStart()
{
	Super::OnStart();


}

void USB_GameInstance::Init()
{
	Super::Init();


}

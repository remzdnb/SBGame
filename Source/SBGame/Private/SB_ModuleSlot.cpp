#include "SB_ModuleSlot.h"
#include "SB_DataManager.h"
//
#include "EngineUtils.h"

USB_ModuleSlot::USB_ModuleSlot()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USB_ModuleSlot::BeginPlay()
{
	Super::BeginPlay();
}

void USB_ModuleSlot::Init(const ASB_DataManager* const NewDataManager)
{
	DataManager = NewDataManager;
}

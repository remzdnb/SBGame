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

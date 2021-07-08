#include "SB_ShipPowerComponent.h"
#include "SB_Ship.h"
#include "SB_BaseModule.h"

USB_ShipPowerComponent::USB_ShipPowerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USB_ShipPowerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USB_ShipPowerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USB_ShipPowerComponent::Init(const ASB_DataManager* const NewDataManager)
{
	DataManager = NewDataManager;
	OwnerShip = Cast<ASB_Ship>(GetOwner());

	for (auto& Module : OwnerShip->GetPowerModules())
	{
		MaxPower += 3;
	}

	StoredPower = MaxPower;

	StoredPowerUpdatedEvent.Broadcast(StoredPower);
}

void USB_ShipPowerComponent::AssignPower(USB_BaseModule* const BaseModuleReference)
{
	/*if (StoredPower > 0 && BaseModuleReference->AddPower())
	{
		StoredPower--;

		StoredPowerUpdatedEvent.Broadcast(StoredPower);
	}*/
}

void USB_ShipPowerComponent::RecoverPower(USB_BaseModule* const BaseModuleReference)
{
	/*if (BaseModuleReference->RemovePower())
	{
		StoredPower++;

		StoredPowerUpdatedEvent.Broadcast(StoredPower);
	}*/

}

#include "SB_BaseModule.h"
#include "SB_Ship.h"
#include "SB_DataManager.h"
//
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

USB_BaseModule::USB_BaseModule()
{
	SetCollisionProfileName("CharacterMesh");
	SetGenerateOverlapEvents(true);
	//SetIsReplicatedByDefault(true);
}

void USB_BaseModule::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerShip = Cast<ASB_Ship>(GetOwner());
}

void USB_BaseModule::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerShip->GetLocalRole() == ROLE_Authority)
	{
		//GetWorld()->GetTimerManager().SetTimer(RepairTimer, this, &USB_BaseModule::RepairOnce, 0.5f, true, 5.0f);
	}
}

void USB_BaseModule::Init(const class ASB_DataManager* const NewDataManager, const FName& NewDataRowName)
{
	DataManager = NewDataManager;
	BaseModuleData = DataManager->GetBaseModuleDataFromRow(NewDataRowName);
	Durability = BaseModuleData->MaxDurability;

	SetAnimInstanceClass(BaseModuleData->AnimInstance);
}

void USB_BaseModule::ApplyDamage(float Damage)
{
	if (Durability > 0)
	{
		if (Durability - Damage <= 0)
		{
			Durability = 0;
			OnModuleDestroyed();
		}
		else
		{
			Durability -= Damage;
		}
	}

	ModuleDurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_BaseModule::RepairOnce()
{
	if (Durability + BaseModuleData->RepairAmount >= BaseModuleData->MaxDurability)
	{
		Durability = BaseModuleData->MaxDurability;
	}
	else
	{
		Durability += BaseModuleData->RepairAmount;
	}

	ModuleDurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_BaseModule::OnModuleDestroyed()
{
}

void USB_BaseModule::OnRep_Durability()
{
	ModuleDurabilityUpdatedEvent.Broadcast(Durability);
}

void USB_BaseModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USB_BaseModule, Durability);
}